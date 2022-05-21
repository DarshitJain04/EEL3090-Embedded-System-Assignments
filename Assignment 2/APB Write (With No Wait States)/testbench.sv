`timescale 1ns / 1ps
module testbenchWRITE;
  reg PCLK, PWRITE, PSEL, PENABLE;
  reg [0:31] PADDR;
  reg [0:31] PWDATA;
  wire PREADY;
  wire [0:31]dataMemory;
  
  APBProtocolWRITE noWait(PCLK, PADDR, PWRITE, PSEL, PENABLE, PWDATA, PREADY, dataMemory);
  
  task writeTransferWithNoWaitStates;
    begin
      // IDLE PHASE
      PCLK = 1; PADDR = 0; PWRITE = 0; PSEL = 0; PENABLE = 0; PWDATA = 0;
      
      // SETUP PHASE
      // PSEL is asserted
      // Until PREADY becomes high, PADDR, PWRITE and PWDATA must be valid/ stable
      @(posedge PCLK);
      	PSEL = 1;
      	PADDR = {$urandom}%32; // Index between 0 and 31
      	PWRITE = 1;
      	PWDATA = {$urandom}%10; // Selecting a random value between 0 and 10

      // State jumps to ACCESS PHASE in the next cycle
      // PENABLE and PREADY are asserted to perform write operations
      @(posedge PCLK);
        PENABLE = 1;
      
      // Assuming no more transfers are required, we jump to IDLE PHASE
      // PENABLE and PSEL are deasserted and other signals too
      @(posedge PCLK);
      	PADDR = 0;
      	PWRITE = 0;
      	PSEL = 0;
        PENABLE = 0;
        PWDATA = 0;
    end
  endtask
  
    initial
      begin
        $dumpfile("dump.vcd");
        $dumpvars;
        writeTransferWithNoWaitStates;
  	  end 
  
  always #20 PCLK = ~PCLK;
  initial #160 $finish;
  
endmodule