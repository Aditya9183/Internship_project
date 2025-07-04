`timescale 1ns / 1ps
module ntt (
    input clk,
    input rst,
    input start,
    output reg done_final,

    // Coeff BRAM
    output reg [7:0] coeff_addr,
    output reg coeff_we,
    output reg [31:0] coeff_din,
    input  [31:0] coeff_dout,

    // Addr BROM
    output reg [7:0] addr_rom_addr,
    input  [7:0] addr_rom_data,

    // Twiddle ROM 
    output reg [7:0] twid_rom_addr,
    input  [31:0] twid_rom_data
);

    parameter Q = 7681;
    parameter STAGES = 3;
    parameter N = 8;
    parameter N_INV = 6721;
    // FSM States
    localparam IDLE         = 4'd0,
               READ_A_ADDR  = 4'd1,
               WAIT_ROM     = 4'd2,
               WAIT_B       = 4'd3,
               SAVE_A_ADDR  = 4'd4,
               WAIT_A_VAL   = 4'd5,
               READ_A_VAL   = 4'd6,
               WAIT_B_VAL   = 4'd7,
               READ_B_VAL   = 4'd8,
               COMPUTE      = 4'd9,
               WRITE_A      = 4'd10,
               WRITE_B      = 4'd11,
               NEXT         = 4'd12,
               FINAL        = 4'd13,
               FINAL_WAIT   = 4'd14,
               FINAL_COMPUTE = 4'd15;

    
    reg [3:0] state;
    reg [3:0] current_stage;
    reg [3:0] group, num_bfu;
    reg [3:0] current_group, current_bfu;
    reg [7:0] final_index;
    reg [7:0] addr_a, addr_b;
    reg [31:0] A, B, T, W;
    reg [31:0] T1, T2;  // for INTT: T1 = A+B, T2 = A-B

    reg [1:0] mode;           // 0 = NTT, 1 = INTT
    reg done,done_all;

    // Modular operations
    function [31:0] modadd;
        input [31:0] a, b;
        begin
            modadd = (a + b) % Q;
        end
    endfunction

    function [31:0] modsub;
        input [31:0] a, b;
        begin
            if (a >= b)
                modsub = a - b;
            else
                modsub = Q + a - b;
        end
    endfunction

    function [31:0] modmul;
        input [31:0] a, b;
        reg [63:0] p;
        begin
            p = a * b;
            modmul = p % Q;
        end
    endfunction

    // FSM
    always @(posedge clk or posedge rst) begin
        if (rst) begin
            coeff_we        <= 0;
            done_all        <= 0;
            done            <= 0;
            done_final      <=0;
            state           <= IDLE;
            addr_rom_addr   <= 0;
            twid_rom_addr   <= 0;
            current_stage   <= 0;
            current_group   <= 0;
            current_bfu     <= 0;
            mode            <= 0;
            final_index     <=0;
        end else begin
            case (state)
                IDLE: begin
                    done_all <= 0;
                    done <= 0;
                    if (start) begin
                        mode <= 0;              
                        group <= 1;
                        num_bfu <= 4;

                        addr_rom_addr <= 0;
                        twid_rom_addr <= 0;
                        current_stage <= 0;
                        current_group <= 0;
                        current_bfu <= 0;
                        state <= READ_A_ADDR;
                    end
                end

                READ_A_ADDR: begin
                    addr_a <= addr_rom_data;
                    addr_rom_addr <= addr_rom_addr + 1;
                    state <= WAIT_ROM;
                end

                WAIT_ROM: state <= WAIT_B;

                WAIT_B: begin
                    addr_b <= addr_rom_data;
                    state <= SAVE_A_ADDR;
                end

                SAVE_A_ADDR: begin
                    addr_rom_addr <= addr_rom_addr + 1;
                    coeff_addr <= addr_a;
                    coeff_we <= 0;
                    state <= WAIT_A_VAL;
                end

                WAIT_A_VAL: state <= READ_A_VAL;

                READ_A_VAL: begin
                    A <= coeff_dout;
                    coeff_addr <= addr_b;
                    state <= WAIT_B_VAL;
                end

                WAIT_B_VAL: state <= READ_B_VAL;

                READ_B_VAL: begin
                    B <= coeff_dout;
                    W <= twid_rom_data;
                    state <= COMPUTE;
                end

                COMPUTE: begin
                    if (mode == 1'b0) begin
                        T <= modmul(B, W);               // NTT: T = W * B
                    end else begin
                        T1 <= modadd(A, B);              // INTT: T1 = A + B
                        T2 <= modsub(A, B);              //        T2 = A - B
                    end
                    state <= WRITE_A;
                end

                WRITE_A: begin
                    coeff_addr <= addr_a;
                    coeff_we <= 1;

                    if (mode == 1'b0)
                        coeff_din <= modadd(A, T);       // A + W·B
                    else
                        coeff_din <=T1;      // (A + B)·??¹

                    state <= WRITE_B;
                end

                WRITE_B: begin
                    coeff_addr <= addr_b;

                    if (mode == 1'b0)
                        coeff_din <= modsub(A, T);       // A - W·B
                    else
                      coeff_din <= modmul(T2, W);      // (A - B)W

                    coeff_we <= 1;
                    state <= NEXT;
                end

                NEXT: begin
                    coeff_we <= 0;

                    if (current_bfu == num_bfu - 1) begin
                        current_bfu <= 0;

                        if (current_group == group - 1) begin
                            current_group <= 0;

                            if (current_stage == STAGES - 1) begin
                                if (mode == 0) begin
                                   //INTT START
                                    mode <= 1;
                                    current_stage <= 0;
                                    group <= 4;
                                    num_bfu <= 1;

                                    
                                    twid_rom_addr <= twid_rom_addr+1;

                                    current_bfu <= 0;
                                    current_group <= 0;

                                    state <= READ_A_ADDR;
                                end else begin
                                    // INTT done
                                    addr_rom_addr<=0;
                                    done_all <= 1;
                                    mode<= 2'b10;
                                    state <= FINAL;
                                end
                            end else begin
                                current_stage <= current_stage + 1;

                                if (mode == 0) begin
                                    group <= group << 1;
                                    num_bfu <= num_bfu >> 1;
                                end else begin
                                    group <= group >> 1;
                                    num_bfu <= num_bfu << 1;
                                end

                                twid_rom_addr <= twid_rom_addr + 1;
                                state <= READ_A_ADDR;
                            end
                        end else begin
                            current_group <= current_group + 1;
                            twid_rom_addr <= twid_rom_addr + 1;
                            state <= READ_A_ADDR;
                        end
                    end else begin
                        current_bfu <= current_bfu + 1;
                        state <= READ_A_ADDR;
                    end
                end
                 FINAL: begin
                    coeff_addr <= final_index;
                    coeff_we <= 0;
                    state <= FINAL_WAIT;
                end
                FINAL_WAIT:begin
                    state<=FINAL_COMPUTE;
                end
                FINAL_COMPUTE: begin
                    coeff_din <= modmul(coeff_dout, N_INV);
                    coeff_addr <= final_index;
                    coeff_we <= 1;

                    if (final_index == N-1) begin
                        done_final <= 1;
                        state <= IDLE;
                    end else begin
                        final_index <= final_index + 1;
                        state <= FINAL;
                    end
                end

                    
                default: state <= IDLE;
            endcase
        end
    end

endmodule
