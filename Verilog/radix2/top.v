module top (
    input clk,
    input rst,
    input start,
    output done
);

    wire [7:0] coeff_addr;
    wire [31:0] coeff_dout, coeff_din;
    wire coeff_we;

    wire [7:0] addr_rom_addr;
    wire [7:0] addr_rom_data;

    wire [7:0] twid_rom_addr;
    wire [31:0] twid_rom_data;

    ntt ntt1 (
        .clk(clk),
        .rst(rst),
        .start(start),
        .done_final(done),

        .coeff_addr(coeff_addr),
        .coeff_we(coeff_we),
        .coeff_din(coeff_din),
        .coeff_dout(coeff_dout),

        .addr_rom_addr(addr_rom_addr),
        .addr_rom_data(addr_rom_data),

        .twid_rom_addr(twid_rom_addr),
        .twid_rom_data(twid_rom_data)
    );

    BROM_0 my_addr_rom (
        .clka(clk),
        .addra(addr_rom_addr),
        .douta(addr_rom_data),
        .ena(1'b1)
    );

    twiddle_rom my_twid_rom (
        .clka(clk),
        .addra(twid_rom_addr),
        .douta(twid_rom_data),
        .ena(1'b1)
    );

    poly_one my_coeff_bram (
        .clka(clk),
        .wea(coeff_we),
        .addra(coeff_addr),
        .dina(coeff_din),
        .douta(coeff_dout)
    );

endmodule
