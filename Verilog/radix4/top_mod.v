`timescale 1ns / 1ps

module top (
    input clk,
    input rst,
    input start,
    output done
);

wire [7:0] addr0, addr1, addr2, addr3;
wire [7:0] coeff_addr0_a, coeff_addr0_b, coeff_addr1_a, coeff_addr1_b;
wire [31:0] coeff0_dout_a, coeff0_dout_b;
wire [31:0] coeff1_dout_a, coeff1_dout_b;

wire [31:0] coeff_din0, coeff_din1, coeff_din2, coeff_din3;
wire        coeff_we;

wire [7:0] twid_rom_addr1, twid_rom_addr2, twid_rom_addr3;
wire [31:0] twid_rom_data1, twid_rom_data2, twid_rom_data3;

wire [7:0] addr_rom_addr0, addr_rom_addr1, addr_rom_addr2, addr_rom_addr3;

ntt_intt ntt_core (
    .clk(clk),
    .rst(rst),
    .start(start),
    .done_final(done),

    // BRAM addresses
    .coeff_addr0_a(coeff_addr0_a),
    .coeff_addr0_b(coeff_addr0_b),
    .coeff_addr1_a(coeff_addr1_a),
    .coeff_addr1_b(coeff_addr1_b),
    
    // BRAM data out
    .coeff0_dout_a(coeff0_dout_a),
    .coeff0_dout_b(coeff0_dout_b),
    .coeff1_dout_a(coeff1_dout_a),
    .coeff1_dout_b(coeff1_dout_b),

    // BRAM data in + write enable
    .coeff_din0(coeff_din0),
    .coeff_din1(coeff_din1),
    .coeff_din2(coeff_din2),
    .coeff_din3(coeff_din3),
    .coeff_we(coeff_we),

    // Twiddle ROMs
    .twid_rom_addr1(twid_rom_addr1),
    .twid_rom_addr2(twid_rom_addr2),
    .twid_rom_addr3(twid_rom_addr3),
    .twid_rom_data1(twid_rom_data1),
    .twid_rom_data2(twid_rom_data2),
    .twid_rom_data3(twid_rom_data3),

    // Address ROMs
    .addr_rom_addr0(addr_rom_addr0),
    .addr_rom_addr1(addr_rom_addr1),
    .addr_rom_addr2(addr_rom_addr2),
    .addr_rom_addr3(addr_rom_addr3),
    .addr0(addr0),
    .addr1(addr1),
    .addr2(addr2),
    .addr3(addr3)
);

// === Dual-port BRAMs ===
coeff_mem1 bram0 (
    .addra(coeff_addr0_a),
    .clka(clk),
    .dina(coeff_din0),
    
    .wea(coeff_we),
    .douta(coeff0_dout_a),

    .addrb(coeff_addr0_b),
    .clkb(clk),
    .dinb(coeff_din1),
    
    .web(coeff_we),
    .doutb(coeff0_dout_b)
);

coeff_mem_two bram1 (
    .addra(coeff_addr1_a),
    .clka(clk),
    .dina(coeff_din2),
    
    .wea(coeff_we),
    .douta(coeff1_dout_a),

    .addrb(coeff_addr1_b),
    .clkb(clk),
    .dinb(coeff_din3),
    
    .web(coeff_we),
    .doutb(coeff1_dout_b)
);

// === Twiddle ROMs ===
blk_mem_gen_0 twid1 (
    .addra(twid_rom_addr1),
    .clka(clk),
    .douta(twid_rom_data1)
);

blk_mem_gen_0 twid2 (
    .addra(twid_rom_addr2),
    .clka(clk),
    .douta(twid_rom_data2)
);

blk_mem_gen_0 twid3 (
    .addra(twid_rom_addr3),
    .clka(clk),
    .douta(twid_rom_data3)
);

// === Address ROM ===
address_rom addrrom0 (
    .addra(addr_rom_addr0), // single address input
    .clka(clk),
    .douta(addr0)
);
address_rom addrrom1 (
    .addra(addr_rom_addr1), // single address input
    .clka(clk),
    .douta(addr1)
);
address_rom addrrom2 (
    .addra(addr_rom_addr2), // single address input
    .clka(clk),
    .douta(addr2)
);
address_rom addrrom3 (
    .addra(addr_rom_addr3), // single address input
    .clka(clk),
    .douta(addr3)
);

endmodule
