
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// State Machine to sequence signals into the SAR ADC and control the peak-detector reset
// This version uses the UDB Datapath as a simple down counter to set the digitizing delay
// Robert Johnson April 2020
//
// Copyright UCSC, 2020
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 04/05/2020 at 18:11
// Component: ACD_CTRL_v1
module ACD_CTRL_v1 (
	output  Done,
	output  MUX,
	output  RstCt,
	output  RstPk,
	output  SOC,
	input   ChOR,
	input   CLK,
	input   EOC,
	input   GO,
	input   NRQ,
	input   Reset,
	input   tc
);
	parameter Period = 15;

//`#start body` -- edit after this line, do not edit this line
// Arguments:
//    Done  - signal for the status register, to indicate that the ADC has been read out twice
//    MUX   - control signal for the analog multiplexer
//    RstCt - reset signal for the external Count7 counter
//    SOC   - signal to the SAR ADC to start a conversion
//    ChOR  - OR of the two channel outputs
//    CLK   - system clock
//    EOC   - signal that the SAR ADC has completed a conversion
//    GO    - AND of the two channel outputs
//    NRQ   - signal that the DMA transfer of the ADC results has completed
//    Reset - reset the state machine
//    tc    - signal indicating termination of the Count7 counter
//    RstPk - reset signal for the peak detectors

reg rstCnt;

wire ZeroDetect;
wire Datapath_1_d0_load;
wire Datapath_1_d1_load;
wire Datapath_1_f0_load;
wire Datapath_1_f1_load;
wire Datapath_1_route_si;
wire Datapath_1_route_ci;
wire  [2:0] Datapath_1_select;

assign Datapath_1_d0_load = (1'b0);
assign Datapath_1_d1_load = (1'b0);
assign Datapath_1_f0_load = (1'b0);
assign Datapath_1_f1_load = (1'b0);
assign Datapath_1_route_si = (1'b0);
assign Datapath_1_route_ci = (1'b0);
assign Datapath_1_select[0] = (ZeroDetect | rstCnt);
assign Datapath_1_select[1] = (1'b0);
assign Datapath_1_select[2] = (1'b0);

/* ==================== Datapath_1 (Width: 8) Instantiation ==================== */
parameter Datapath_1_dpconfig0 = 
{
    `CS_ALU_OP__DEC, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC__ALU, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM0: Decrement */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC___D0, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM1: Reset */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM2:  */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM3:  */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM4:  */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM5:  */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM6:  */
    `CS_ALU_OP_PASS, `CS_SRCA_A0, `CS_SRCB_D0, `CS_SHFT_OP_PASS, `CS_A0_SRC_NONE, `CS_A1_SRC_NONE, `CS_FEEDBACK_DSBL, `CS_CI_SEL_CFGA, `CS_SI_SEL_CFGA, `CS_CMP_SEL_CFGA,  /*CFGRAM7:  */
    8'hFF, 8'h00, /* CFG9 */
    8'hFF, 8'hFF, /* CFG11-10 */
    `SC_CMPB_A1_D1, `SC_CMPA_A0_D1, `SC_CI_B_ARITH, `SC_CI_A_ARITH, `SC_C1_MASK_DSBL, `SC_C0_MASK_DSBL, `SC_A_MASK_DSBL, `SC_DEF_SI_0, `SC_SI_B_DEFSI, `SC_SI_A_DEFSI, /* CFG13-12 */
    `SC_A0_SRC_ACC, `SC_SHIFT_SL, 1'b0, `SC_SR_SRC_REG, `SC_FIFO1_BUS, `SC_FIFO0_BUS, `SC_MSB_ENBL, `SC_MSB_BIT7, `SC_MSB_NOCHN, `SC_FB_NOCHN, `SC_CMP1_NOCHN, `SC_CMP0_NOCHN, /* CFG15-14 */
    3'b000, `SC_FIFO_SYNC__ADD, 2'b000, `SC_FIFO1_DYN_OF, `SC_FIFO0_DYN_OF, `SC_FIFO_CLK1_POS, `SC_FIFO_CLK0_POS, `SC_FIFO_CLK__DP, `SC_FIFO_CAP_AX, `SC_FIFO_LEVEL, `SC_FIFO__SYNC, `SC_EXTCRC_DSBL, `SC_WRK16CAT_DSBL /* CFG17-16 */
};
cy_psoc3_dp8 #(
    .cy_dpconfig_a( Datapath_1_dpconfig0 ),
    .d0_init_a( Period ),
    .d1_init_a( 8'b00000000 ),
    .a0_init_a( Period ),
    .a1_init_a( 8'b00000000 ))
    Datapath_1(
        .clk( CLK ),
        .cs_addr( Datapath_1_select ),
        .route_si( Datapath_1_route_si ),
        .route_ci( Datapath_1_route_ci ),
        .f0_load( Datapath_1_f0_load ),
        .f1_load( Datapath_1_f1_load ),
        .d0_load( Datapath_1_d0_load ),
        .d1_load( Datapath_1_d1_load ),
        .ce0(  ), 
        .cl0(  ), 
        .z0( ZeroDetect ), 
        .ff0(  ), 
        .ce1(  ), 
        .cl1(  ), 
        .z1(  ), 
        .ff1(  ), 
        .ov_msb(  ), 
        .co_msb(  ), 
        .cmsb(  ), 
        .so(  ), 
        .f0_bus_stat(  ), 
        .f0_blk_stat(  ), 
        .f1_bus_stat(  ), 
        .f1_blk_stat(  )
    );

reg MUX2, SOC2, RstCt2, Done2;
assign MUX = MUX2;
assign SOC = SOC2;
assign RstCt = RstCt2;
assign Done = Done2;

reg [2:0] State, NextState;

parameter [2:0] Wait = 3'b000;  // Wait for a signal from either detector
parameter [2:0] Dlay = 3'b001;  // Delay to look for the GO and for the peak detector signal to stabilize
parameter [2:0] Soc0 = 3'b010;  // Send first start of conversion signal
parameter [2:0] Eoc0 = 3'b011;  // Wait for the first ADC to finish
parameter [2:0] Soc1 = 3'b100;  // Send second start of conversion signal
parameter [2:0] Eoc1 = 3'b101;  // Wait for the second ADC to finish
parameter [2:0] Wnrq = 3'b110;  // Wait for the last DMA transfer to complete, and set status register
parameter [2:0] Fini = 3'b111;  // Send out the done signal to reset peak detectors after OR signal is gone

assign RstPk = (State == Fini);

reg GOlatch, TClatch;
always @ (State or GO or GOlatch or TClatch or ChOR or EOC or NRQ or tc) begin
    case (State) 
      Wait: begin
                if (ChOR) NextState = Dlay;
                else NextState = Wait;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
                rstCnt = 1'b1;
            end
      Dlay: begin
                if (ZeroDetect) begin
                    if (GOlatch) NextState = Soc0;
                    else NextState = Fini;
                end else NextState = Dlay;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
                rstCnt = 1'b0;
            end
      Soc0: begin
                NextState = Eoc0;
                MUX2 = 1'b0;
                SOC2 = 1'b1;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
                rstCnt = 1'b1;
            end
      Eoc0: begin
                if (EOC) NextState = Soc1;
                else NextState = Eoc0;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
                rstCnt = 1'b1;
            end
      Soc1: begin
                NextState = Eoc1;
                MUX2 = 1'b1;
                SOC2 = 1'b1;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
                rstCnt = 1'b1;
            end
      Eoc1: begin
                if (EOC) begin
                    NextState = Wnrq;
                end else begin
                    NextState = Eoc1;
                end
                RstCt2 = 1'b1;
                MUX2 = 1'b1;
                SOC2 = 1'b0;               
                Done2 = 1'b0;
                rstCnt = 1'b1;
            end
      Wnrq: begin
                if (NRQ) begin  
                    NextState = Fini;
                    Done2 = 1'b1;                   
                end else begin
                    NextState = Wnrq;
                    Done2 = 1'b0;
                end
                RstCt2 = 1'b1;
                MUX2 = 1'b1;
                SOC2 = 1'b0;      
                rstCnt = 1'b1;
            end
      Fini: begin
                if (!ChOR & TClatch) NextState = Wait;
                else NextState = Fini;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b0;
                Done2 = 1'b0;
                rstCnt = 1'b1;
            end
    endcase
end

always @ (posedge CLK) begin
    if (Reset) begin
        State <= Wait;
        GOlatch <= 1'b0;
    end else begin
        State <= NextState;
        if (State == Fini) begin  // Capture the GO signal if and when it arrives.
            GOlatch <= 1'b0;
            if (tc) TClatch <= 1'b1;
        end else begin
            TClatch <= 1'b0;
            if (GO) GOlatch <= 1'b1;
        end
    end
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
