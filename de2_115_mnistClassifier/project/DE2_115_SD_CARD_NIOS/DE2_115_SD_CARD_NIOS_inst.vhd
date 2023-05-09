	component DE2_115_SD_CARD_NIOS is
		port (
			altpll_areset_conduit_export                                    : in    std_logic                     := 'X';             -- export
			altpll_c1_clk                                                   : out   std_logic;                                        -- clk
			altpll_c3_clk                                                   : out   std_logic;                                        -- clk
			altpll_locked_conduit_export                                    : out   std_logic;                                        -- export
			c0_out_clk_clk                                                  : out   std_logic;                                        -- clk
			c2_out_clk_clk                                                  : out   std_logic;                                        -- clk
			clk_50_clk_in_clk                                               : in    std_logic                     := 'X';             -- clk
			key_external_connection_export                                  : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			ledg_external_connection_export                                 : out   std_logic_vector(8 downto 0);                     -- export
			ledr_external_connection_export                                 : out   std_logic_vector(17 downto 0);                    -- export
			reset_reset_n                                                   : in    std_logic                     := 'X';             -- reset_n
			sd_clk_external_connection_export                               : out   std_logic;                                        -- export
			sd_cmd_external_connection_export                               : inout std_logic                     := 'X';             -- export
			sd_dat_external_connection_export                               : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			sd_wp_n_external_connection_export                              : in    std_logic                     := 'X';             -- export
			sw_external_connection_export                                   : in    std_logic_vector(17 downto 0) := (others => 'X'); -- export
			tri_state_bridge_flash_bridge_0_out_address_to_the_cfi_flash    : out   std_logic_vector(22 downto 0);                    -- address_to_the_cfi_flash
			tri_state_bridge_flash_bridge_0_out_tri_state_bridge_flash_data : inout std_logic_vector(7 downto 0)  := (others => 'X'); -- tri_state_bridge_flash_data
			tri_state_bridge_flash_bridge_0_out_write_n_to_the_cfi_flash    : out   std_logic_vector(0 downto 0);                     -- write_n_to_the_cfi_flash
			tri_state_bridge_flash_bridge_0_out_select_n_to_the_cfi_flash   : out   std_logic_vector(0 downto 0);                     -- select_n_to_the_cfi_flash
			tri_state_bridge_flash_bridge_0_out_read_n_to_the_cfi_flash     : out   std_logic_vector(0 downto 0);                     -- read_n_to_the_cfi_flash
			video_vga_controller_0_external_interface_CLK                   : out   std_logic;                                        -- CLK
			video_vga_controller_0_external_interface_HS                    : out   std_logic;                                        -- HS
			video_vga_controller_0_external_interface_VS                    : out   std_logic;                                        -- VS
			video_vga_controller_0_external_interface_BLANK                 : out   std_logic;                                        -- BLANK
			video_vga_controller_0_external_interface_SYNC                  : out   std_logic;                                        -- SYNC
			video_vga_controller_0_external_interface_R                     : out   std_logic_vector(7 downto 0);                     -- R
			video_vga_controller_0_external_interface_G                     : out   std_logic_vector(7 downto 0);                     -- G
			video_vga_controller_0_external_interface_B                     : out   std_logic_vector(7 downto 0);                     -- B
			results_input_external_connection_export                        : in    std_logic_vector(31 downto 0) := (others => 'X'); -- export
			weight_index_external_connection_export                         : out   std_logic_vector(12 downto 0);                    -- export
			pixel_index_external_connection_export                          : out   std_logic_vector(9 downto 0);                     -- export
			sync_data_external_connection_export                            : out   std_logic_vector(15 downto 0);                    -- export
			activations_index_external_connection_export                    : out   std_logic_vector(3 downto 0);                     -- export
			floatdata_output_external_connection_export                     : out   std_logic_vector(31 downto 0)                     -- export
		);
	end component DE2_115_SD_CARD_NIOS;

	u0 : component DE2_115_SD_CARD_NIOS
		port map (
			altpll_areset_conduit_export                                    => CONNECTED_TO_altpll_areset_conduit_export,                                    --                     altpll_areset_conduit.export
			altpll_c1_clk                                                   => CONNECTED_TO_altpll_c1_clk,                                                   --                                 altpll_c1.clk
			altpll_c3_clk                                                   => CONNECTED_TO_altpll_c3_clk,                                                   --                                 altpll_c3.clk
			altpll_locked_conduit_export                                    => CONNECTED_TO_altpll_locked_conduit_export,                                    --                     altpll_locked_conduit.export
			c0_out_clk_clk                                                  => CONNECTED_TO_c0_out_clk_clk,                                                  --                                c0_out_clk.clk
			c2_out_clk_clk                                                  => CONNECTED_TO_c2_out_clk_clk,                                                  --                                c2_out_clk.clk
			clk_50_clk_in_clk                                               => CONNECTED_TO_clk_50_clk_in_clk,                                               --                             clk_50_clk_in.clk
			key_external_connection_export                                  => CONNECTED_TO_key_external_connection_export,                                  --                   key_external_connection.export
			ledg_external_connection_export                                 => CONNECTED_TO_ledg_external_connection_export,                                 --                  ledg_external_connection.export
			ledr_external_connection_export                                 => CONNECTED_TO_ledr_external_connection_export,                                 --                  ledr_external_connection.export
			reset_reset_n                                                   => CONNECTED_TO_reset_reset_n,                                                   --                                     reset.reset_n
			sd_clk_external_connection_export                               => CONNECTED_TO_sd_clk_external_connection_export,                               --                sd_clk_external_connection.export
			sd_cmd_external_connection_export                               => CONNECTED_TO_sd_cmd_external_connection_export,                               --                sd_cmd_external_connection.export
			sd_dat_external_connection_export                               => CONNECTED_TO_sd_dat_external_connection_export,                               --                sd_dat_external_connection.export
			sd_wp_n_external_connection_export                              => CONNECTED_TO_sd_wp_n_external_connection_export,                              --               sd_wp_n_external_connection.export
			sw_external_connection_export                                   => CONNECTED_TO_sw_external_connection_export,                                   --                    sw_external_connection.export
			tri_state_bridge_flash_bridge_0_out_address_to_the_cfi_flash    => CONNECTED_TO_tri_state_bridge_flash_bridge_0_out_address_to_the_cfi_flash,    --       tri_state_bridge_flash_bridge_0_out.address_to_the_cfi_flash
			tri_state_bridge_flash_bridge_0_out_tri_state_bridge_flash_data => CONNECTED_TO_tri_state_bridge_flash_bridge_0_out_tri_state_bridge_flash_data, --                                          .tri_state_bridge_flash_data
			tri_state_bridge_flash_bridge_0_out_write_n_to_the_cfi_flash    => CONNECTED_TO_tri_state_bridge_flash_bridge_0_out_write_n_to_the_cfi_flash,    --                                          .write_n_to_the_cfi_flash
			tri_state_bridge_flash_bridge_0_out_select_n_to_the_cfi_flash   => CONNECTED_TO_tri_state_bridge_flash_bridge_0_out_select_n_to_the_cfi_flash,   --                                          .select_n_to_the_cfi_flash
			tri_state_bridge_flash_bridge_0_out_read_n_to_the_cfi_flash     => CONNECTED_TO_tri_state_bridge_flash_bridge_0_out_read_n_to_the_cfi_flash,     --                                          .read_n_to_the_cfi_flash
			video_vga_controller_0_external_interface_CLK                   => CONNECTED_TO_video_vga_controller_0_external_interface_CLK,                   -- video_vga_controller_0_external_interface.CLK
			video_vga_controller_0_external_interface_HS                    => CONNECTED_TO_video_vga_controller_0_external_interface_HS,                    --                                          .HS
			video_vga_controller_0_external_interface_VS                    => CONNECTED_TO_video_vga_controller_0_external_interface_VS,                    --                                          .VS
			video_vga_controller_0_external_interface_BLANK                 => CONNECTED_TO_video_vga_controller_0_external_interface_BLANK,                 --                                          .BLANK
			video_vga_controller_0_external_interface_SYNC                  => CONNECTED_TO_video_vga_controller_0_external_interface_SYNC,                  --                                          .SYNC
			video_vga_controller_0_external_interface_R                     => CONNECTED_TO_video_vga_controller_0_external_interface_R,                     --                                          .R
			video_vga_controller_0_external_interface_G                     => CONNECTED_TO_video_vga_controller_0_external_interface_G,                     --                                          .G
			video_vga_controller_0_external_interface_B                     => CONNECTED_TO_video_vga_controller_0_external_interface_B,                     --                                          .B
			results_input_external_connection_export                        => CONNECTED_TO_results_input_external_connection_export,                        --         results_input_external_connection.export
			weight_index_external_connection_export                         => CONNECTED_TO_weight_index_external_connection_export,                         --          weight_index_external_connection.export
			pixel_index_external_connection_export                          => CONNECTED_TO_pixel_index_external_connection_export,                          --           pixel_index_external_connection.export
			sync_data_external_connection_export                            => CONNECTED_TO_sync_data_external_connection_export,                            --             sync_data_external_connection.export
			activations_index_external_connection_export                    => CONNECTED_TO_activations_index_external_connection_export,                    --     activations_index_external_connection.export
			floatdata_output_external_connection_export                     => CONNECTED_TO_floatdata_output_external_connection_export                      --      floatdata_output_external_connection.export
		);

