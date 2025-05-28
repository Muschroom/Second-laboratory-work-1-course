#include <iostream>
#include <string>
#include <functional>
#include "ArraySequence.h"
#include "ListSequence.h"
#include "AdaptiveSequence.h"
#include "SigmentedList.h"
using namespace std;



template <typename T>
class SequenceTester {
private:
    Sequence<T>* sequence;
    
    void printMenu() {
        cout << "\nOperations" << endl;
        cout << "1. GetFirst" << endl;
        cout << "2. GetLast" << endl;
        cout << "3. Get" << endl;
        cout << "4. GetSubsequence" << endl;
        cout << "5. GetLength" << endl;
        cout << "6. Append" << endl;
        cout << "7. Prepend" << endl;
        cout << "8. InsertAt" << endl;
        cout << "9. Concat" << endl;
        cout << "10. Map" << endl;
        cout << "11. Reduce" << endl;
        cout << "12. Where" << endl;
        cout << "13. Print" << endl;
        cout << "0. Exit" << endl;
        cout << "Choice: ";
    }

    void printSequence(Sequence<T>* seq) {
        for (int i = 0; i < seq->GetLength(); ++i) {
            cout << seq->Get(i);
            if (i != seq->GetLength() - 1) {
                cout << " ";
            }
        }
    }

    T getValue() {
        T value;
        cout << "Value: ";
        cin >> value;
        return value;
    }

    int getIndex(int max) {
        int index;
        while (true) {
            cout << "Enter index (0-" << max-1 << "): ";
            cin >> index;
            if (index >= 0 && index < max) {
                return index;
            }
            cout << "Invalid index!" << endl;
        }
    }
  
    std::function<T(T)> MapFunc() {
        return [](T x) { return x; }; 
    }

    std::function<bool(T)> WhereFunc() {
        return [](T x) { return bool(); }; 
    }

    std::function<T(T, T)> ReduceFunc() {
        return [](T a, T b) { return a + b; };
    }

public:
    SequenceTester(Sequence<T>* seq) : sequence(seq) {}

    void run() {
        int choice;
        while (true) {
            printMenu();
            cin >> choice;

            try {
                switch (choice) {
                    case 1:
                        cout << "First element: " << sequence->GetFirst() << endl;
                        break;
                    case 2:
                        cout << "Last element: " << sequence->GetLast() << endl;
                        break;
                    case 3: {
                        int index = getIndex(sequence->GetLength());
                        cout << "Element at " << index << ": " << sequence->Get(index) << endl;
                        break;
                    }
                    case 4: {
                        int start = getIndex(sequence->GetLength());
                        int end = getIndex(sequence->GetLength());
                        if (start > end) {
                            cout << "Start index must be <= end index!" << endl;
                            break;
                        }
                        Sequence<T>* sub = sequence->GetSubsequence(start, end);
                        cout << "Subsequence: ";
                        printSequence(sub);
                        delete sub;
                        break;
                    }
                    case 5:
                        cout << "Length: " << sequence->GetLength() << endl;
                        break;

                    case 6: { 
                        T value = getValue();
                        sequence = sequence->Append(value);
                        cout << "Value appended." << endl;
                        break;
                    }
                        
                    case 7: { 
                        T value = getValue();
                        sequence = sequence->Prepend(value);
                        cout << "Value prepended." << endl;
                        break;
                    }
                        
                    case 8: {
                        T value = getValue();
                        int index = getIndex(sequence->GetLength() + 1);
                        sequence = sequence->InsertAt(value, index);
                        cout << "Value inserted." << endl;
                        break;
                    }
                    case 9: {
                        cout << "Sequence to concatenate" << endl;
                        Sequence<T>* other = createSequence();
                        sequence = sequence->Concat(other);
                        cout << "Sequences concatenated." << endl;
                        delete other;
                        break;
                    }
                    case 10: {
                        Sequence<T>* mapped = sequence->Map(MapFunc());
                        cout << "Mapped sequence: ";
                        printSequence(mapped);
                        delete mapped;
                        break;
                    }
                    case 11: {
                        T value = getValue();
                        T result = sequence->Reduce(ReduceFunc(), value);
                        cout << "Reduction result: " << result << endl;
                        break;
                    }
                    case 12: {
                        Sequence<T>* filtered = sequence->Where(WhereFunc());
                        cout << "Filtered sequence: ";
                        printSequence(filtered);
                        delete filtered;
                        break;
                    }
                    case 13:
                        cout << "Current sequence: ";
                        printSequence(sequence);
                        break;
                    case 0:
                        return;
                    default:
                        cout << "Invalid choice!" << endl;
                }
            } catch (const exception& e) {
                cout << "Error: " << e.what() << endl;
            }
        }
    }

    static Sequence<T>* createSequence() {
        cout << "\n Type \n" << endl;
        cout << "1. Mutable ArraySequence" << endl;
        cout << "2. Immutable ArraySequence" << endl;
        cout << "3. Mutable ListSequence" << endl;
        cout << "4. Immutable ListSequence" << endl;
        cout << "5. AdaptiveSequence" << endl;
        cout << "6. SegmentedList" << endl;
        cout << "Choice: ";
        
        int typeChoice;
        cin >> typeChoice;

        cout << "Number of elements: ";
        int count;
        cin >> count;

        T* items = new T[count];
        for (int i = 0; i < count; ++i) {
            cout << "Element " << i << ": ";
            cin >> items[i];
        }
        int segmentSize = 0;
        if(typeChoice == 6){
            cout << "Enter segment size: ";
            cin >> segmentSize;
        }
        Sequence<T>* seq = nullptr;
        switch (typeChoice) {
            case 1:
                seq = new MutableArraySequence<T>(items, count);
                break;
            case 2:
                seq = new ImmutableArraySequence<T>(items, count);
                break;
            case 3:
                seq = new MutableListSequence<T>(items, count);
                break;
            case 4:
                seq = new ImmutableListSequence<T>(items, count);
                break;
            case 5:
                seq = new AdaptiveSequence<T>(items, count);
                break;
            case 6:
                seq = new SegmentedList<T>(items, count, segmentSize);
                break;
            default:
                delete[] items;
                throw invalid_argument("Invalid sequence type choice");
        }

        delete[] items;
        return seq;
    }
};

template <>
std::function<int(int)> SequenceTester<int>::MapFunc() {
    return [](int x) { return x + 1; };
}

template <>
std::function<std::string(std::string)> SequenceTester<std::string>::MapFunc() {
    return [](std::string s) { return "LOL" + s; };  
}


template <>
std::function<double(double)> SequenceTester<double>::MapFunc() {
    return [](double d) { return d + 1; };  
}

template <>
std::function<bool(int)> SequenceTester<int>::WhereFunc() {
    return [](int x) { return x > 2; };
}

template <>
std::function<bool(string)> SequenceTester<std::string>::WhereFunc() {
    return [](std::string s) { return s.size() % 2 == 0; };  
}

template <>
std::function<bool(double)> SequenceTester<double>::WhereFunc() {
    return [](double d) { return d + 1 > 5; };  
}

template <>
std::function<int(int, int)> SequenceTester<int>::ReduceFunc() {
    return [](int a, int b) { return a + b; }; 
}

template <>
std::function<std::string(std::string, std::string)> SequenceTester<std::string>::ReduceFunc() {
    return [](std::string a, std::string b) { return a + " " + b; };
}

template <>
std::function<double(double, double)> SequenceTester<double>::ReduceFunc() {
    return [](double a, double b) { return a * b; }; 
}