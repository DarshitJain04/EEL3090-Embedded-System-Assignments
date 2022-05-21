#include <bits/stdc++.h>

using namespace std;

// Keeps track of number of peripheral devices
int numberOfPeripherals = 0;

// Maximum number of instructions in the Program Memory
int programMemorySize = 110;

// Register used in instruction execution
int registerR0 = 0;

// Total number of fundamental instructions in the Instruction Set Architecture
int numberOfInstructions = 14;

// Stack to store and restore microprocessor state
stack<int> store;

// key: PC Location in Program Memory, value: instruction
unordered_map<int, int> programMemory;

// Blueprint of a peripheral device
class Peripheral{
    public:
        string name = "Peripheral ";
        int data;
        Peripheral(){
            numberOfPeripherals++;
            name += to_string(numberOfPeripherals);
            cout<<"Connected "<<name<<" to the processor\n";
        }   
};

// Blueprint of the Microprocessor 
class Microprocessor{
    public:
        int programCounter;
        int interruptServiceRoutineLocation;
        bool interruptFlag = false;
        Microprocessor(){
            interruptServiceRoutineLocation = 16;
            programCounter = 90;
            std::printf("\nInterrupt Service Routine location in Program Memory: %d\n", interruptServiceRoutineLocation);
            std::printf("Main application program location in Program Memory: %d\n", programCounter);
            std::printf("Initialized Program Counter to: %d\n\n", programCounter);
        }
};

// Initializes Program Memory (Instructions for Interrupt Service Routine and Main Application Program)
void initiateProgramMemory(Microprocessor &microprocessor){
    int currentProgramCounter = microprocessor.programCounter;
    int ISRLocation = microprocessor.interruptServiceRoutineLocation;

    // Interrupt Service Routine
    programMemory[ISRLocation] = 11;
    ISRLocation++;
    programMemory[ISRLocation] = 4;
    ISRLocation++;
    programMemory[ISRLocation] = 11;
    ISRLocation++;
    programMemory[ISRLocation] = 14;

    // Main Application Program
    while(currentProgramCounter <= programMemorySize){
        programMemory[currentProgramCounter] = rand()%numberOfInstructions;
        currentProgramCounter++;
    }

    std::printf("\nInitiated Program Memory with size: %d\n\n", programMemorySize);
}

// Executes the Interrupt Service Routine 
void interruptServiceRoutine(Peripheral &peripheral1, Peripheral &peripheral2, Microprocessor &microprocessor){
    std::printf("\n*** EXECUTING INTERRUPT SERVICE ROUTINE ***\n\n");

    // Program Counter at PC = 16
    microprocessor.programCounter = microprocessor.interruptServiceRoutineLocation;
    std::printf("Current Program Counter (PC): %d\n", microprocessor.programCounter);
    std::printf("Move Instruction: registerR0 = dataMemory[address1]\n\n");
    microprocessor.programCounter++;
    registerR0 = peripheral1.data;

    // Program Counter at PC = 17
    std::printf("Current Program Counter (PC): %d\n", microprocessor.programCounter);
    std::printf("Logical Left Shift (by 1 bit) Instruction: registerR0 = registerR0 << 1\n\n");
    microprocessor.programCounter++;
    registerR0 = registerR0 << 1;

    // Program Counter at PC = 18
    std::printf("Current Program Counter (PC): %d\n", microprocessor.programCounter);
    std::printf("Move Instruction: dataMemory[address2] = registerR0\n\n");
    microprocessor.programCounter++;
    peripheral2.data = registerR0;

    // Program Counter at PC = 19
    std::printf("Current Program Counter (PC): %d\n", microprocessor.programCounter);
    std::printf("Halt Instruction: Stop current instructions execution\n\n");
    return;
}

// Executes intruction pointed by the Program Counter
// 0: No Operation Instruction
// 1: Bitwise AND Instruction
// 2: Bitwise OR Instruction
// 3: Bitwise XOR Instruction 
// 4: Logical Left shift by 1 bit Instruction
// 5: Logical Right shift by 1 bit Instruction
// 6: Add Instruction
// 7: Add Immediately Instruction
// 8: Multiply Instruction
// 9: Divide Instruction
// 10: Swap Instruction
// 11: Move Instruction
// 12: Jump Instruction
// 13: Store Pogram Counter Instruction
// 14: Halt Instruction
void ProgramMemory(Peripheral &peripheral1, Peripheral &peripheral2, Microprocessor &microprocessor){
    if(microprocessor.interruptFlag){
        interruptServiceRoutine(peripheral1, peripheral2, microprocessor);
        return;
    }
    std::printf("Current Program Counter (PC): %d\n", microprocessor.programCounter);
    switch (programMemory[microprocessor.programCounter]) {
        case 0:
            std::printf("NOP Instruction: No Operation\n\n");
            break;
        case 1:
            std::printf("Bitwise AND Instruction: dataMemory[address1] = dataMemory[address1] & dataMemory[address2]\n\n");
            break;
        case 2:
            std::printf("Bitwise OR Instruction: dataMemory[address1] = dataMemory[address1] | dataMemory[address2]\n\n");
            break;
        case 3:
            std::printf("Bitwise XOR Instruction: dataMemory[address1] = dataMemory[address1] ^ dataMemory[address2]\n\n");
            break;
        case 4:
            std::printf("Logical Left Shift (by 1 bit) Instruction: dataMemory[address1] = dataMemory[address1] << 1\n\n");
            break;
        case 5:
            std::printf("Logical Right Shift (by 1 bit) Instruction: dataMemory[address1] = dataMemory[address1] >> 1\n\n");
            break;
        case 6:
            std::printf("Add Instruction: dataMemory[address1] = dataMemory[address1] + dataMemory[address2]\n\n");
            break;
        case 7:
            std::printf("Add Immediate Instruction: dataMemory[address1] = dataMemory[address1] + Immediate data\n\n");
            break;
        case 8:
            std::printf("Multiplication Instruction: dataMemory[address1] = dataMemory[address1] * dataMemory[address2]\n\n");
            break;
        case 9:
            std::printf("Divide Instruction: dataMemory[address1] = dataMemory[address1] / dataMemory[address2]\n\n");
            return;
        case 10:
            std::printf("Swap Instruction:\registerR0 = dataMemory[address1]\ndataMemory[address1] = dataMemory[address2]\ndataMemory[address2] = registerR0\n\n");
            break;
        case 11:
            std::printf("Move Instruction: dataMemory[address1] = dataMemory[address2]\n\n");
            break;
        case 12:
            std::printf("Jump Instruction: Program Counter (PC) = address1\n\n");
            break;
        case 13:
            std::printf("Store Pogram Counter Instruction: dataMemory[address1] = PC (Program Counter)\n\n");break;
        case 14:
            std::printf("Halt Instruction: Stop instructions execution\n\n");
            break;
        default:
            return;
    }
}

int main()
{
    // Create and instance of microprocessor
    Microprocessor microprocessor = Microprocessor();

    // Create and instance of peripheral device: Peripheral 1
    Peripheral peripheral1 = Peripheral();

    // Create and instance of peripheral device: Peripheral 2
    Peripheral peripheral2 = Peripheral();

    // Initiate Porgram Memory with instructions for ISR and Main Application Program
    initiateProgramMemory(microprocessor);

    // Waiting for input in another thread (Read a token from the standard input stream using >> in this thread)
    auto task = std::async(std::launch::async, [] {
        int input;
        if (std::cin >> input){
            return input;
        }
        return -1;
    });

    // Continue processor execution in the main thread while waiting for input from the parallel thread
    while(task.wait_for(2s) != std::future_status::ready && microprocessor.programCounter <= programMemorySize) {
        ProgramMemory(peripheral1, peripheral2, microprocessor);
        microprocessor.programCounter++;
    }


    // If received an input, create and interrupt, stop current execution and store microporcessor state
    if(task.wait_for(2s) == std::future_status::ready){

        // Input data in peripheral 1
        peripheral1.data = task.get();

        std::printf("Received input from Periheral 1: %d\n", peripheral1.data);
        std::printf("\nStoring previous state (Program Counter, Interrupt Flag and Register) in memory stack\n");

        // Store current microporcessor state in memory stack
        store.push(microprocessor.interruptFlag);
        store.push(microprocessor.programCounter);
        store.push(registerR0);

        // Shows that the microprocessor has received an interrupt
        microprocessor.interruptFlag = true;

        // Microprocessor will jump to the fixed ISR location at Program Memory location 16, i.e., PC = 16
        microprocessor.programCounter = microprocessor.interruptServiceRoutineLocation;
        std::printf("\nInitiating interrupt\n");

        ProgramMemory(peripheral1, peripheral2, microprocessor);

        std::printf("\nOutput stored in Peripheral 2: %d\n", peripheral2.data);
        std::printf("\nRestoring microprocessor state and continuing execution\n\n");

        // Retore microporcessor state
        registerR0 = store.top();
        store.pop();
        microprocessor.programCounter = store.top();
        store.pop();
        microprocessor.interruptFlag = store.top();
        store.pop();

        // Microporcessor resumes executing its program 
        while (microprocessor.programCounter <= programMemorySize)
        {
            ProgramMemory(peripheral1, peripheral2, microprocessor);
            microprocessor.programCounter++;
        }

        std::printf("\n***Program reached end of the Program Memory. Stopping execution!***\n");
    }else{
        std::printf("\n***Program reached end of the Program Memory with no input received. Stopping execution!***\n");
    }
    
    return 0;
}