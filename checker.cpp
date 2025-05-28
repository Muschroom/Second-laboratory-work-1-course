#include "headers/Checker.h"





int main() {
    cout << "Type" << endl;
    cout << "1. int" << endl;
    cout << "2. string" << endl;
    cout << "3. double" << endl;
    cout << "Choice: ";
    
    int dataType;
    cin >> dataType;
    switch (dataType) {
        case 1: {
            SequenceTester<int> tester(SequenceTester<int>::createSequence());
            tester.run();
            break;
        }
        case 2: {
            SequenceTester<std::string> tester(SequenceTester<std::string>::createSequence());
            tester.run();
            break;
        }
        case 3: {
            SequenceTester<double> tester(SequenceTester<double>::createSequence());
            tester.run();
            break;
        }
        default:
            cout << "Invalid data type choice!" << endl;
            return 1;
    }

}
