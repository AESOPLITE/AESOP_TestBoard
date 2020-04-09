
//`#start header` -- edit after this line, do not edit this line
// ========================================
//
// Copyright YOUR COMPANY, THE YEAR
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF your company.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 03/21/2020 at 09:01
// Component: SignalCrop_v1
module SignalCrop_v1 (
	output  Y,
	input   A,
	input   Clk,
	input   Reset,
	input   tc
);

//`#start body` -- edit after this line, do not edit this line

//        Your code goes here
parameter [1:0] Wait = 2'b00;
parameter [1:0] Cont = 2'b01;
parameter [1:0] Done = 2'b10;
parameter [1:0] Null = 2'b11;

reg [1:0] State, NextState;

assign Y = (State == Cont);
always @ (State or A) begin
    case (State) 
	    Wait: begin
		          if (A) NextState = Cont;
				  else NextState = Wait;
		      end
	    Cont: begin
		          if (tc) NextState = Done;
				  else NextState = Cont;
		      end
		Done: begin
		          if (A) NextState = Done;
				  else NextState = Wait;
		      end
		Null: begin
		          NextState = Wait;
		      end
	endcase
end

always @ (posedge Clk) begin
    if (Reset) begin
	    State <= Wait;
	end else begin
	    State <= NextState;
	end
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
