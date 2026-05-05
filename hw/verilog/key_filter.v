module key_filter(
        Clk,
        Reset_n,
        Key,
        Key_R_Flag,
        Key_P_Flag,
        Key_State
    );
    input Clk;
    input Reset_n;
    input Key;
    output reg Key_P_Flag;
    output reg Key_R_Flag;
    output reg Key_State;

    reg [1:0]state;

    localparam IDLE=0;
    localparam P_FILTER=1;
    localparam WAIT_R=2;
    localparam R_FILTER=3;

    parameter TIME_UNIT_MS = 1000;
    parameter MCNT = 2500*TIME_UNIT_MS-1;
    reg [21:0]cnt;

    // ??“å…©???
    reg sync_d0_Key;
    reg sync_d1_Key;
    reg r_Key;

    wire pedge_key;
    wire nedge_key;

    wire time_20ms_reached;
    assign time_20ms_reached=cnt>=MCNT;

    always@(posedge Clk)
        sync_d0_Key<=Key;

    always@(posedge Clk)
        sync_d1_Key<=sync_d0_Key;

    always@(posedge Clk)
        r_Key<=sync_d1_Key;

    assign nedge_key=(sync_d1_Key==0)&&(r_Key==1);
    assign pedge_key=(sync_d1_Key==1)&&(r_Key==0);

    // ????‹æ??
    always@(posedge Clk or negedge Reset_n)
        if(!Reset_n) begin
            state<=IDLE;
            Key_P_Flag<=1'd0;
            Key_R_Flag<=1'd0;
            cnt<=0;
            Key_State<=1'd1;
        end
        else
        case (state)
            IDLE: begin
                Key_R_Flag<=0;
                if(nedge_key)
                    state<=P_FILTER;
            end
            P_FILTER:
                if(time_20ms_reached) begin
                    state<=WAIT_R;
                    Key_P_Flag<=1;
                    Key_State<=1'd0;
                    cnt<=0;
                end
                else if(pedge_key) begin
                    state<=IDLE;
                    cnt<=0;
                end
                else begin
                    state<=state;
                    cnt<=cnt+1'd1;
                end
            WAIT_R: begin
                Key_P_Flag<=1'd0;
                if(pedge_key)
                    state<=R_FILTER;
            end
            R_FILTER:
                if(time_20ms_reached) begin
                    state<=IDLE;
                    Key_State<=1'd1;
                    Key_R_Flag<=1'd1;
                    cnt<=0;
                end
                else if(nedge_key) begin
                    state<=WAIT_R;
                    cnt<=0;
                end
                else begin
                    state<=state;
                    cnt<=cnt+1'd1;
                end
        endcase
endmodule

