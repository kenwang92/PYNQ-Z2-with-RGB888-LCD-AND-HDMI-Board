module emio_div (
    input  [12:0] Gpio,
    output [ 7:0] gpio_ps,
    output        rst,
    output        bl_ctr,
    output        spi_sclk,
    output        spi_mosi,
    output        spi_cs
);
    assign gpio_ps  = Gpio[7:0];
    assign bl_ctr   = Gpio[8];
    assign rst      = Gpio[9];
    assign spi_sclk = Gpio[10];
    assign spi_mosi = Gpio[11];
    assign spi_cs   = Gpio[12];
endmodule
