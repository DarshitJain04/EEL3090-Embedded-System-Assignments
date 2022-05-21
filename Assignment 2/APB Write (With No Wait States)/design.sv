`timescale 1ns / 1ps
module APBProtocolWRITE(PCLK, PADDR, PWRITE, PSEL, PENABLE, PWDATA, PREADY, dataMemory);
  input PCLK, PWRITE, PSEL, PENABLE;
  input [0:31] PADDR;
  input [0:31] PWDATA;
	
  reg [0:1] currentPhase = 0;
  reg [0:1] nextPhase = 0;
  
  output reg PREADY;
  output reg [0:31]dataMemory;
  
  always @(posedge PCLK)
    begin
      currentPhase <= nextPhase;
      if(nextPhase == 0 && $time != 160000)
        begin
          $display("Current Phase: IDLE, %0t", $time);
        end
      else if(nextPhase == 1 && $time != 160000)
        begin
          $display("Current Phase: SETUP, %0t", $time);
        end
      else if(nextPhase == 2 && $time != 160000)
        begin
          $display("Current Phase: ACCESS, %0t", $time);
        end
    end

  // Perform operations whenever the Current Phase changes
  always @(currentPhase)
    begin
      // IDLE PHASE
      if(currentPhase == 0)
        begin
          PREADY = 0;
          nextPhase <= 1;
        end
      // SETUP PHASE
      else if(currentPhase == 1)
        begin
          nextPhase <= 2;
        end
      // ACCESS PHASE
      else if(currentPhase == 2 && PSEL == 1 && PENABLE == 0)
        begin
          PREADY = 1;
			    nextPhase <= 1; // Move to SETUP PHASE if more transfers are remaining
        end
      else
        begin
          PREADY = 1;
          dataMemory = PWDATA;
			    nextPhase <= 0; // Move to IDLE PHASE if more transfers are remaining
        end
    end
endmodule