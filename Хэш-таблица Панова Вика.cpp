#include <iostream>
#include <string>
#include <vector>
#include <list>


enum SweetType {
    Candy,
    Cake,
    Cookie, 
    UnknownType
};


enum Flavour {
    Vanilla,
    Strawberry,
    Chocolate,
    Mint,
    Lemon,
    Other
};


class Sweets {
public:
    std::string name;    
    SweetType type;      
    Flavour flavour;     
    size_t calories;        
    size_t proteins;         

    Sweets() : name(""), type(SweetType::UnknownType), flavour(Flavour::Other), calories(0), proteins(0) {}

    Sweets(const std::string& name, SweetType type, Flavour flavour, int calories, int proteins)
        : name(name), type(type), flavour(flavour), calories(calories), proteins(proteins) {}


    void print() const {
        std::cout << "Sweet: " << name <<
                     "\nType: " << type << 
                     "\nFlavour: " << flavour << 
                     "\nCalories: " << calories <<
                     "\nProteins:" << proteins << std::endl;
    }
};

class HashMap {
private:
    struct KeyValuePair {
        std::string key;
        Sweets value;
        bool isOccupied;
        bool isDeleted;

        KeyValuePair() : isOccupied(false), isDeleted(false) {}
        KeyValuePair(const std::string& k, const Sweets& v, bool occupied, bool deleted)
        : key(k), value(v), isOccupied(occupied), isDeleted(deleted) {}
    };

    std::vector<KeyValuePair> table;
    size_t capacity;
    size_t size;
    float loadFactorThreshold;


    size_t hash(const std::string& key) const {
        size_t hashValue = 0;
        for (char c : key) {
            hashValue = hashValue * 31 + c; 
        }
        return hashValue % capacity;
    }


    size_t findIndex(const std::string& key) const {
        size_t index = hash(key);
        size_t start_index = index;
        size_t steps = 0;
    
    
        while (steps < capacity) {
        
            if (!table[index].isOccupied || table[index].isDeleted)
                break;
                
            
            if (table[index].key == key)
                break;
    
            index = (index + 1) % capacity;
            steps++;
        }
    
        return index;
    }
 
    void resizeAndRehash() {
        size_t newCapacity = capacity * 2;
        std::vector<KeyValuePair> newTable(newCapacity);

        for (const auto& pair : table) {
            if (pair.isOccupied && !pair.isDeleted) {
                size_t index = hash(pair.key) % newCapacity;
                while (newTable[index].isOccupied) {
                    index = (index + 1) % newCapacity;
                }
                newTable[index] = pair;
            }
        }

        table = std::move(newTable);
        capacity = newCapacity;
    }

public:
       HashMap(size_t initialCapacity = 10, float loadFactor = 0.7f)
        : capacity(initialCapacity), size(0), loadFactorThreshold(loadFactor) {
        table.resize(capacity);
    }

 
    void insert(const std::string& key, const Sweets& value) {
        if (static_cast<float>(size) / capacity > loadFactorThreshold) {
            resizeAndRehash();
        }
    
        size_t index = findIndex(key);
    
        if (!table[index].isOccupied || table[index].isDeleted) {
            table[index] = KeyValuePair{key, value, true, false};
            size++;
        } else if (table[index].key == key) {
            table[index].value = value; 
        } else {
            
            resizeAndRehash();
            insert(key, value);
        }
    }
  
    bool find(const std::string& key, Sweets& value) const {
        size_t index = findIndex(key);
        
        if (table[index].isOccupied && 
            !table[index].isDeleted && 
            table[index].key == key) 
        {
            value = table[index].value;
            return true;
        }
        return false;
    }

    
    bool remove(const std::string& key) {
        size_t index = findIndex(key);

        if (table[index].isOccupied && !table[index].isDeleted) {
            table[index].isDeleted = true; 
            size--;
            return true;
        }
        return false;
    }

    void print() const {
        std::cout << "HashMap contents:\n";
        for (size_t i = 0; i < capacity; ++i) {
            if (table[i].isOccupied && !table[i].isDeleted) {
                std::cout << "Index " << i << ": "
                          << "Key: " << table[i].key << ", "
                          << "Sweet: " << table[i].value.name
                          << ", Type: " << static_cast<int>(table[i].value.type)
                          << ", Flavour: " << static_cast<int>(table[i].value.flavour)
                          << ", Calories: " << table[i].value.calories
                          << std::endl;
            }
        }
    }
    ~HashMap() {}
    
   
    void clear() {
    for (size_t i = 0; i < capacity; ++i) {
        table[i].isOccupied = false;
        table[i].isDeleted = false;
    }
    size = 0;  
}

};

int main() {
    HashMap hashMap;

  
    hashMap.insert("Chocolate Cake", Sweets("Chocolate Cake", Cake, Chocolate, 250, 10));
    hashMap.insert("Vanilla Ice Cream", Sweets("Vanilla Ice Cream", SweetType::UnknownType, Vanilla, 200, 5));
    hashMap.insert("Mint Chocolate", Sweets("Mint Chocolate", SweetType::Candy, Flavour::Mint, 150, 7));

 
    Sweets foundSweet;
    if (hashMap.find("Chocolate Cake", foundSweet)) {
        foundSweet.print();
    }


    hashMap.remove("Vanilla Ice Cream");
    std::cout << "After removal:\n";
    hashMap.print();

    return 0;
}
