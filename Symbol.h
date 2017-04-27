 #ifndef SYMBOL_H
 #define SYMBOL_H

 #include<string>
 #include<vector>

 class Symbol {
  private:
  //Class Variables
    std::string label; //label is just the label name for label portion of the source code
    int Address, Length; //Address is the location of the symbol being stored.  Length is the length of the symbol in bytes

  public:


  //Constructor
    Symbol(std::string Label, int address, int length);

  //Destructor
    ~Symbol();

  //Class Methods
    std::string getLabel();
    void setLabel(std::string symbol);
    int getAddress();
    void setAddress(int address);
    int getLength();
    void setLength(int length);

	}
 #endif
