# Input/output
    use argument 
    input filename
# class and method
    * class prediction
        * public 
            using func_type = function<void(int, int, int)>
            using instruction = tuple<func_type, int, int, int>
            using instruction_set = vector<instruction>
            * struct istate 
                * int err = 0
                * state s
                > use to store state
                > err means error times
            * prediction(int size)
                > constructor with size of entry
                > bind function with instruction name
                > resize BHT
            * void parseText(vector<string>)
                > parse program text to a runable program
            * void run_inst()
                > run program and try to prediction
        
        * private
            * int pc
                > programe counter 
            * bool isBrh
                > for branch
            * bool jmp
                > for jump
            * instruction_set instructions
                > program
            * vector<string> text
                > input text with no label
            * vector<istate> BHT
                > BHT with (int, state)
            * vector<int> reg
                > register
            * map<string, func_type> inst
                > instruction with (int, int, int) types parameter
            * void set(string str, int first, int second, int third)
                > push instruction into inst
                > string str: instruction name
                > int first, int second, int third: each three parameter
            * istate& getState(int i)
                > get state of entry i
            
            * void beq(int, int, int)
            * void li(int, int, int)
            * void addi(int, int, int)
            
            * void jump(int);
                > change pc and jmp
            * bool getReg(string&, int&)
                > if it is a reg get register
                > else if it is a label , get label position
                > else return value
    * class state2bit
        * state2bit& operator++()
            > increase s until s = 11
        * state2bit& operator--()
            > decrease s until s = 00
        * operator bool() const
            > return value of this : N or T
        * bool operator!() const
            > return inverse value
        * char* getState()
            > get state , ST, WT, WT, and ST

    * class state
        * public 
            * state()
                > set defalt 00
            * state& operator++()
                > call state2bit::operator++()
            * state& operator--()
                > call state2bit::operator--()
            * operator bool() const
                > return value of current state
            * bool operator!() const
                > return inverse value of current state
            * void push(bool b)
                > push value to stateBits
            * void print()
        * private
            * bool stateBits[2]
                > for current state
            * state2bit stateList[4]
                > 2 bit state list

# algorithm
    1. record label and remove form a line, remove tabs, and remove empty line
    2. make instruction be a format like
        beq(5, 6, -2) or li(1, 0, 0) // `li R1, 0` will be add a no use parameter
    3. because the bind function can make function and instruction name be linked, program just run every function  
    4. if the instruction will jump to label will call jump(int), this function will make run_inst() know that state is changed, and then call operater++ or operater--

#description
    almost function just make convinience to dealing, important
    run_inst() will record last_pc, if pc isn't the same with pc which was predicted by BHT, increase error time and change state

