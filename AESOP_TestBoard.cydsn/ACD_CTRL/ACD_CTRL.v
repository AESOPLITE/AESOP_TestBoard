
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// State Machine to sequence signals into the SAR ADC and control the peak-detector reset
// Robert Johnson March 2020
//
// Copyright UCSC, 2020
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF UCSC.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 03/25/2020 at 11:11
// Component: ACD_CTRL
module ACD_CTRL (
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
reg [2:0] cnt;

// Note: the external Count7 time must be longer than the longest conversion time.
// This restriction could be removed if we can be sure the the EOC signal will always be received,
// so that the state machine cannot get stuck forever in an Eoc state.
reg GOlatch, TClatch;
always @ (State or GO or GOlatch or TClatch or ChOR or EOC or NRQ or tc) begin
    case (State) 
      Wait: begin
                if (ChOR) NextState = Dlay;
                else NextState = Wait;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b0;
                Done2 = 1'b0;
            end
      Dlay: begin
                if (cnt == 3'b111) begin
                    if (GOlatch) NextState = Soc0;
                    else NextState = Fini;
                end else NextState = Dlay;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b0;
                Done2 = 1'b0;
            end
      Soc0: begin
                NextState = Eoc0;
                MUX2 = 1'b0;
                SOC2 = 1'b1;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
            end
      Eoc0: begin
                if (EOC) NextState = Soc1;
                else if (tc) NextState = Fini; // This is to prevent getting stuck forever here
                else NextState = Eoc0;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b0;
                Done2 = 1'b0;
            end
      Soc1: begin
                NextState = Eoc1;
                MUX2 = 1'b1;
                SOC2 = 1'b1;
                RstCt2 = 1'b1;
                Done2 = 1'b0;
            end
      Eoc1: begin
                if (EOC) begin
                    NextState = Wnrq;
                end else if (tc) begin
                    NextState = Fini; // This is to prevent getting stuck forever here
                end else begin
                    NextState = Eoc1;
                end
                RstCt2 = 1'b1;
                MUX2 = 1'b1;
                SOC2 = 1'b0;               
                Done2 = 1'b0;
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
            end
      Fini: begin
                if (!ChOR & TClatch) NextState = Wait;
                else NextState = Fini;
                MUX2 = 1'b0;
                SOC2 = 1'b0;
                RstCt2 = 1'b0;
                Done2 = 1'b0;
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
            if (State == Wait) cnt <= 3'b000;
            else cnt <= cnt + 1;
        end
    end
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
