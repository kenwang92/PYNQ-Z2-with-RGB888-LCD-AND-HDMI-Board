//=============================================================================
// Resolution Select (Uncomment ONLY ONE)
//=============================================================================
// `define Res_480x272   1// 9M
// `define Res_640x480   1// 25M
// `define Res_800x480   1// 33M
`define Res_480x800   1// 33M
// `define Res_800x600 1// 40M
// `define Res_1024x600  1// 51M
// `define Res_1024x768  1// 65M
// `define Res_1280x720  1// 74.25M
// `define Res_1920x1080 1// 148.5M

`define RGB_888 1
// `define RGB_565 1
// `define RGB_444 1

`ifdef RGB_888
`define Red 8
`define Green 8
`define Blue 8
`elsif RGB_565
`define Red 5
`define Green 6
`define Blue 5
`elsif RGB_444
`define Red 4
`define Green 4
`define Blue 4
`endif
//=============================================================================
// Timing Parameters
//=============================================================================

`ifdef Res_480x272
`define DISP_CLK 9_000_000
`define H_RIGHT_BORDER 12'd0
`define H_FRONT_PORCH 12'd2
`define H_SYNC_TIME 12'd41
`define H_BACK_PORCH 12'd2
`define H_LEFT_BORDER 12'd0
`define H_DATA 12'd480
`define H_TOTAL 12'd525

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd2
`define V_SYNC_TIME 12'd10
`define V_BACK_PORCH 12'd2
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd272
`define V_TOTAL 12'd286

`elsif Res_640x480
`define DISP_CLK 25_000_000
`define H_RIGHT_BORDER 12'd8
`define H_FRONT_PORCH 12'd8
`define H_SYNC_TIME 12'd96
`define H_BACK_PORCH 12'd40
`define H_LEFT_BORDER 12'd8
`define H_DATA 12'd640
`define H_TOTAL 12'd800

`define V_BOTTOM_BORDER 12'd8
`define V_FRONT_PORCH 12'd2
`define V_SYNC_TIME 12'd2
`define V_BACK_PORCH 12'd25
`define V_TOP_BORDER 12'd8
`define V_DATA 12'd480
`define V_TOTAL 12'd525

`elsif Res_800x480
`define DISP_CLK 33_000_000
`define H_RIGHT_BORDER 12'd0
`define H_FRONT_PORCH 12'd40
`define H_SYNC_TIME 12'd128
`define H_BACK_PORCH 12'd88
`define H_LEFT_BORDER 12'd0
`define H_DATA 12'd800
`define H_TOTAL 12'd1056

`define V_BOTTOM_BORDER 12'd8
`define V_FRONT_PORCH 12'd2
`define V_SYNC_TIME 12'd2
`define V_BACK_PORCH 12'd25
`define V_TOP_BORDER 12'd8
`define V_DATA 12'd480
`define V_TOTAL 12'd525

`elsif Res_480x800
`define DISP_CLK 33_000_000
`define H_RIGHT_BORDER 12'd8
`define H_FRONT_PORCH 12'd2
`define H_SYNC_TIME 12'd2
`define H_BACK_PORCH 12'd25
`define H_LEFT_BORDER 12'd8
`define H_DATA 12'd480
`define H_TOTAL 12'd525

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd40
`define V_SYNC_TIME 12'd128
`define V_BACK_PORCH 12'd88
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd800
`define V_TOTAL 12'd1056

`elsif Res_800x600
`define DISP_CLK 40_000_000
`define H_RIGHT_BORDER 12'd0// ok
`define H_FRONT_PORCH 12'd40// ok
`define H_SYNC_TIME 12'd128// ok
`define H_BACK_PORCH 12'd88// ok
`define H_LEFT_BORDER 12'd0// ok
`define H_DATA 12'd800// ok
`define H_TOTAL 12'd1056// ok

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd1
`define V_SYNC_TIME 12'd4// ok
`define V_BACK_PORCH 12'd23
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd600
`define V_TOTAL 12'd628

`elsif Res_1024x600
`define DISP_CLK 51_000_000
`define H_RIGHT_BORDER 12'd0
`define H_FRONT_PORCH 12'd24
`define H_SYNC_TIME 12'd136
`define H_BACK_PORCH 12'd160
`define H_LEFT_BORDER 12'd0
`define H_DATA 12'd1024
`define H_TOTAL 12'd1344

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd1
`define V_SYNC_TIME 12'd4
`define V_BACK_PORCH 12'd23
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd600
`define V_TOTAL 12'd628

`elsif Res_1024x768
`define DISP_CLK 65_000_000
`define H_RIGHT_BORDER 12'd0
`define H_FRONT_PORCH 12'd24
`define H_SYNC_TIME 12'd136
`define H_BACK_PORCH 12'd160
`define H_LEFT_BORDER 12'd0
`define H_DATA 12'd1024
`define H_TOTAL 12'd1344

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd3
`define V_SYNC_TIME 12'd6
`define V_BACK_PORCH 12'd29
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd768
`define V_TOTAL 12'd806

`elsif Res_1280x720
`define DISP_CLK 74_250_000
`define H_RIGHT_BORDER 12'd0
`define H_FRONT_PORCH 12'd110
`define H_SYNC_TIME 12'd40
`define H_BACK_PORCH 12'd220
`define H_LEFT_BORDER 12'd0
`define H_DATA 12'd1280
`define H_TOTAL 12'd1650

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd5
`define V_SYNC_TIME 12'd5
`define V_BACK_PORCH 12'd20
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd720
`define V_TOTAL 12'd750

`elsif Res_1920x1080
`define DISP_CLK 148_500_000
`define H_RIGHT_BORDER 12'd0
`define H_FRONT_PORCH 12'd88
`define H_SYNC_TIME 12'd44
`define H_BACK_PORCH 12'd148
`define H_LEFT_BORDER 12'd0
`define H_DATA 12'd1920
`define H_TOTAL 12'd2200

`define V_BOTTOM_BORDER 12'd0
`define V_FRONT_PORCH 12'd4
`define V_SYNC_TIME 12'd5
`define V_BACK_PORCH 12'd36
`define V_TOP_BORDER 12'd0
`define V_DATA 12'd1080
`define V_TOTAL 12'd1125
`endif
