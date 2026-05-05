module key_4_led_4 (
    input        Clk,
    input        Reset_n,
    input  [3:0] keys,
    output [3:0] leds,

    output [7:0] gpio_ps_i,
    input  [7:0] gpio_ps_o
);
    wire [3:0] filtered_keys;

    genvar i;
    generate
        for (i = 0; i < 4; i = i + 1) begin : key_gen
            key_filter u_filter (
                .Clk       (Clk),
                .Reset_n   (Reset_n),
                .Key       (keys[i]),
                .Key_R_Flag(),
                .Key_P_Flag(),
                .Key_State (filtered_keys[i])
            );
        end
    endgenerate
    assign gpio_ps_i = {4'd0, filtered_keys[3:0]};
    assign leds      = gpio_ps_o[7:4];
endmodule
