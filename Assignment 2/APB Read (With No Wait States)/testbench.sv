`timescale 1ns / 1ps
module testbenchWRITE;
  reg PCLK, PWRITE, PSEL, PENABLE;
  reg [0:31] PADDR;
  reg [0:31] dataMemory;
  wire PREADY;
  wire [0:31] PRDATA;
  
  APBProtocolREAD noWait(PCLK, PADDR, PWRITE, PSEL, PENABLE, PRDATA, PREADY, dataMemory);
  
  task readTransferWithNoWaitStates;
    begin
      // IDLE PHASE
      PCLK = 1; PADDR = 0; PWRITE = 1; PSEL = 0; PENABLE = 0; dataMemory = {$urandom}%10;
      
      // SETUP PHASE
      // PSEL is asserted
      // Until PREADY becomes high, PADDR and PWRITE must be valid/ stable
      @(posedge PCLK);
      	PSEL = 1;
      	PENABLE = 0;
      	PADDR = {$urandom}%32; // Index between 0 and 31
      	PWRITE = 0; // Selecting a random value between 0 and 10

      // State jumps to ACCESS PHASE in the next cycle
      // PENABLE and PREADY are asserted to perform write operations
      @(posedge PCLK);
        PENABLE = 1;
      
      // Assuming no more transfers are required, we jump to IDLE PHASE
      // PENABLE and PSEL are deasserted and other signals too
      @(posedge PCLK);
      	PADDR = 0;
      	PWRITE = 1;
      	PSEL = 0;
        PENABLE = 0;
    end
  endtask
  
    initial
      begin
        $dumpfile("dump.vcd");
        $dumpvars;
        readTransferWithNoWaitStates;
  	  end 
  
  always #20 PCLK = ~PCLK;
  initial #160 $finish;
  
endmodule