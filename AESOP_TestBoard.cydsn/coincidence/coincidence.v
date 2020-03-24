
//`#start header` -- edit after this line, do not edit this line
// ========================================
// Looks for a coincidence of two digital signals within a specified window.
// Coincidences outside of the window get ignored. If either input goes high
// but there is no coincidence, the state machine will wait until both inputs
// are low before looking again for a coincidence.
// Once a coincidence is found, the state machine waits for a specified
// number of clock cycles and then outputs a pulse of specified width.
// No new coincidence search is initiated until both inputs go low.
//
// R. Johnson
// Copyright UCSC, 2019
// All Rights Reserved
// UNPUBLISHED, LICENSED SOFTWARE.
//
// CONFIDENTIAL AND PROPRIETARY INFORMATION
// WHICH IS THE PROPERTY OF UC Santa Cruz.
//
// ========================================
`include "cypress.v"
//`#end` -- edit above this line, do not edit this line
// Generated on 12/20/2019 at 11:33
// Component: coincidence
module coincidence (
	output  Clr,
	output  Y,
	input   A,
	input   B,
	input   Clock,
	input   Reset
);
	parameter ClearTime = 15;
	parameter Delay = 4;
	parameter PulseWidth = 4;
	parameter Window = 4;

//`#start body` -- edit after this line, do not edit this line

// Enumeration of the states of the state machine
parameter [2:0] Wait = 3'b001;   // Wait for either input to go high
parameter [2:0] Look = 3'b010;   // Look for a coincidence within the window
parameter [2:0] Clrp = 3'b011;   // Send signal to clear the peak detector
parameter [2:0] Dela = 3'b100;   // Delay before signaling to start the ADC
parameter [2:0] Outp = 3'b101;   // Output a signal to start the ADC
parameter [2:0] Done = 3'b110;   // Wait until both signals go low before looking for a new coincidence

reg [2:0] State, NextState;
reg [3:0] cnt;

reg Y2;
assign Y = Y2;

reg C2;
assign Clr = C2;

always @ (State or A or B or cnt) begin
    case (State)
        Wait: begin
		         if (A & B) NextState = Dela;
                 else if (A | B) NextState = Look;
                 else NextState = Wait;
                 C2 = 1'b0;
                 Y2 = 1'b0;
              end
        Look: begin
                 if (A & B) NextState = Dela;
				 else if (cnt == Window) NextState = Clrp;
                 else NextState = Look;
                 C2 = 1'b0;
                 Y2 = 1'b0;
              end
        Clrp: begin
                 if (cnt == ClearTime) NextState = Done;
                 else NextState = Clrp;
                 C2 = 1'b1;
                 Y2 = 1'b0;
              end
		Dela: begin
		         if (cnt == Delay) NextState = Outp;
				 else NextState = Dela;
                 C2 = 1'b0;
                 Y2 = 1'b0;
		      end
		Outp: begin
		         if (cnt == PulseWidth) NextState = Done;
				 else NextState = Outp;
                 C2 = 1'b0;
                 Y2 = 1'b1;
		      end
        Done: begin
                 if (!(A | B)) NextState = Wait;
                 else NextState = Done;
                 C2 = 1'b0;
                 Y2 = 1'b0;
              end
        default: begin  
                    NextState = Wait;
                    C2 = 1'b0;
                    Y2 = 1'b0;
                 end
    endcase
end

always @ (posedge Clock) begin
    if (Reset) begin
        State <= Wait;
    end else begin
	    State <= NextState;
		//$display("%g\t State=%b A=%b B=%b cnt=%d Y=%b",$time,State,A,B,cnt,Y);
        case (State)
            Wait: begin
			          if (A & B) cnt <= 2;
                      else cnt <= 1;
                  end
            Look: begin
					  if (A & B) cnt <= 2;
                      else if (cnt == Window) cnt <= 0;
					  else cnt <= cnt + 1;
                  end
            Clrp: begin
                      cnt <= cnt + 1;
                  end
			Dela: begin
			          if (cnt == Delay) cnt <= 1;
			          else cnt <= cnt + 1;
			      end
			Outp: begin
			          cnt <= cnt +1;
			      end
        endcase
    end     
end
//`#end` -- edit above this line, do not edit this line
endmodule
//`#start footer` -- edit after this line, do not edit this line
//`#end` -- edit above this line, do not edit this line
