
module DE2_115_SD_CARD_NIOS (
	altpll_areset_conduit_export,
	altpll_c1_clk,
	altpll_c3_clk,
	altpll_locked_conduit_export,
	c0_out_clk_clk,
	c2_out_clk_clk,
	clk_50_clk_in_clk,
	key_external_connection_export,
	ledg_external_connection_export,
	ledr_external_connection_export,
	reset_reset_n,
	sd_clk_external_connection_export,
	sd_cmd_external_connection_export,
	sd_dat_external_connection_export,
	sd_wp_n_external_connection_export,
	sw_external_connection_export,
	tri_state_bridge_flash_bridge_0_out_address_to_the_cfi_flash,
	tri_state_bridge_flash_bridge_0_out_tri_state_bridge_flash_data,
	tri_state_bridge_flash_bridge_0_out_write_n_to_the_cfi_flash,
	tri_state_bridge_flash_bridge_0_out_select_n_to_the_cfi_flash,
	tri_state_bridge_flash_bridge_0_out_read_n_to_the_cfi_flash,
	video_vga_controller_0_external_interface_CLK,
	video_vga_controller_0_external_interface_HS,
	video_vga_controller_0_external_interface_VS,
	video_vga_controller_0_external_interface_BLANK,
	video_vga_controller_0_external_interface_SYNC,
	video_vga_controller_0_external_interface_R,
	video_vga_controller_0_external_interface_G,
	video_vga_controller_0_external_interface_B,
	results_input_external_connection_export,
	weight_index_external_connection_export,
	pixel_index_external_connection_export,
	sync_data_external_connection_export,
	activations_index_external_connection_export,
	floatdata_output_external_connection_export);	

	input		altpll_areset_conduit_export;
	output		altpll_c1_clk;
	output		altpll_c3_clk;
	output		altpll_locked_conduit_export;
	output		c0_out_clk_clk;
	output		c2_out_clk_clk;
	input		clk_50_clk_in_clk;
	input	[3:0]	key_external_connection_export;
	output	[8:0]	ledg_external_connection_export;
	output	[17:0]	ledr_external_connection_export;
	input		reset_reset_n;
	output		sd_clk_external_connection_export;
	inout		sd_cmd_external_connection_export;
	inout	[3:0]	sd_dat_external_connection_export;
	input		sd_wp_n_external_connection_export;
	input	[17:0]	sw_external_connection_export;
	output	[22:0]	tri_state_bridge_flash_bridge_0_out_address_to_the_cfi_flash;
	inout	[7:0]	tri_state_bridge_flash_bridge_0_out_tri_state_bridge_flash_data;
	output	[0:0]	tri_state_bridge_flash_bridge_0_out_write_n_to_the_cfi_flash;
	output	[0:0]	tri_state_bridge_flash_bridge_0_out_select_n_to_the_cfi_flash;
	output	[0:0]	tri_state_bridge_flash_bridge_0_out_read_n_to_the_cfi_flash;
	output		video_vga_controller_0_external_interface_CLK;
	output		video_vga_controller_0_external_interface_HS;
	output		video_vga_controller_0_external_interface_VS;
	output		video_vga_controller_0_external_interface_BLANK;
	output		video_vga_controller_0_external_interface_SYNC;
	output	[7:0]	video_vga_controller_0_external_interface_R;
	output	[7:0]	video_vga_controller_0_external_interface_G;
	output	[7:0]	video_vga_controller_0_external_interface_B;
	input	[31:0]	results_input_external_connection_export;
	output	[12:0]	weight_index_external_connection_export;
	output	[9:0]	pixel_index_external_connection_export;
	output	[15:0]	sync_data_external_connection_export;
	output	[3:0]	activations_index_external_connection_export;
	output	[31:0]	floatdata_output_external_connection_export;
endmodule
