`include "disp_parameter_cfg.v"
`define RGB888
// `define RGB565
// `define RGB444
module tft_lcd_vdma (
    input                          Clk_DISP,
    input                          Reset_n,
    output                         DISP_HS,
    output                         DISP_VS,
    output                         DISP_DE,
    output [`Red+`Green+`Blue-1:0] D,
    // for VDMA
    input                          valid_in,
    input  [  `Red+`Green+`Blue:0] fifo_data_in
);
    reg  [`Red+`Green+`Blue-1:0] Data;
    wire [                 11:0] h_addr;
    wire [                 11:0] v_addr;
    reg                          tuser_r;
    reg                          tvalid_r;

    disp_driver disp_driver_inst0 (
        .CLK_DISP(Clk_DISP),
        .Reset_n (Reset_n),
        .Data    (Data),
        .tuser   (tuser_r),
        .tvalid  (tvalid_r),
        .h_addr  (h_addr),
        .v_addr  (v_addr),
        .DISP_HS (DISP_HS),
        .DISP_VS (DISP_VS),
        .DISP_DE (DISP_DE),
        .D       (D)
    );
`ifdef RGB888
    localparam BLACK = 24'h000000;
    localparam RED = 24'hFF0000;
    localparam GREEN = 24'h00FF00;
    localparam BLUE = 24'h0000FF;
    localparam YELLOW = 24'hFFFF00;
    localparam AQUA = 24'h00FFFF;
    localparam PINK = 24'hFF00FF;
    localparam WHITE = 24'hFFFFFF;
`elsif RGB444
    localparam BLACK = 12'h000;
    localparam RED = 12'hF00;
    localparam GREEN = 12'h0F0;
    localparam BLUE = 12'h00F;
    localparam YELLOW = 12'hFF0;
    localparam AQUA = 12'h0FF;
    localparam PINK = 12'hF0F;
    localparam WHITE = 12'hFFF;
`endif

    always @(posedge Clk_DISP or negedge Reset_n) begin
        if (~Reset_n) begin
            tuser_r <= 0;
        end else begin
            tuser_r <= fifo_data_in[`Red+`Green+`Blue];
        end
    end

    // tvalid connect with fifo empty
    // when empty=1, set tvalid 0
    always @(posedge Clk_DISP or negedge Reset_n) begin
        if (~Reset_n) begin
            tvalid_r <= 0;
        end else begin
            tvalid_r <= ~valid_in;
        end
    end

    always @(posedge Clk_DISP or negedge Reset_n) begin
        if (~Reset_n) begin
            Data <= 0;
        end else begin
            if (tvalid_r && DISP_DE) begin
                Data <= fifo_data_in[`Red+`Green+`Blue-1:0];
            end else begin
                Data <= Data;
            end
        end
    end

endmodule
