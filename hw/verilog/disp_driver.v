`include "disp_parameter_cfg.v"
module disp_driver (
    input                          CLK_DISP,
    input                          Reset_n,
    input  [`Red+`Green+`Blue-1:0] Data,
    input                          tuser,
    input                          tvalid,
    output [                 11:0] h_addr,
    output [                 11:0] v_addr,
    output                         DISP_DE,
    output                         DISP_HS,
    output                         DISP_VS,
    output [`Red+`Green+`Blue-1:0] D
);

    reg  [11:0] hcnt;
    reg  [11:0] vcnt;
    reg         disp_de_r;
    reg         tuser_d1;
    wire        h_scan;
    wire        v_scan;
    wire        frame_start;

    parameter H_DATA_BEGIN = `H_SYNC_TIME + `H_BACK_PORCH + `H_LEFT_BORDER;
    parameter H_DATA_END = `H_TOTAL - `H_RIGHT_BORDER - `H_FRONT_PORCH;
    parameter V_DATA_BEGIN = `V_SYNC_TIME + `V_BACK_PORCH + `V_TOP_BORDER;
    parameter V_DATA_END = `V_TOTAL - `V_BOTTOM_BORDER - `V_FRONT_PORCH;

    // 只有在有效數據輸出範圍計數
    assign h_addr      = (disp_de_r) ? (hcnt - H_DATA_BEGIN) : 12'd0;
    assign v_addr      = (disp_de_r) ? (vcnt - V_DATA_BEGIN) : 12'd0;

    assign h_scan      = (hcnt >= `H_TOTAL - 1);
    assign v_scan      = (vcnt >= `V_TOTAL - 1);

    assign DISP_DE     = disp_de_r;
    assign frame_start = tuser && ~tuser_d1 && tvalid;

    always @(posedge CLK_DISP or negedge Reset_n)
        if (~Reset_n) hcnt <= 0;
        else if (h_scan || frame_start) hcnt <= 0;
        else hcnt <= hcnt + 1'b1;

    always @(posedge CLK_DISP or negedge Reset_n)
        if (~Reset_n) vcnt <= 0;
        else if (frame_start) vcnt <= 0;
        else if (h_scan)
            if (v_scan) vcnt <= 0;
            else vcnt <= vcnt + 1'b1;
        else vcnt <= vcnt;
    // 有效數據輸出範圍
    always @(posedge CLK_DISP or negedge Reset_n)
        if (~Reset_n) disp_de_r <= 0;
        else
            disp_de_r<=((hcnt>=H_DATA_BEGIN)&&
        (hcnt<H_DATA_END)&&
        (vcnt>=V_DATA_BEGIN)&&
        (vcnt<V_DATA_END));

    always @(posedge CLK_DISP or negedge Reset_n) begin
        if (~Reset_n) begin
            tuser_d1 <= 0;
        end else begin
            tuser_d1 <= tuser;
        end
    end


    // 組合邏輯輸出
    assign DISP_HS = (hcnt > `H_SYNC_TIME - 1);
    assign DISP_VS = (vcnt > `V_SYNC_TIME - 1);
    assign D       = (disp_de_r) ? Data : 0;
endmodule
