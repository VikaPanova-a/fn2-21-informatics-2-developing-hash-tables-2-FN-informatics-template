# Семинар 2
# Панова Виктория ФН2-21Б Вариант 21 Открытая адресация: линейное пробирование
## 1. Заголовочные файлы и перечисления (enum):
``` c++ #include <iostream>
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
```
## 2.Класс Sweets:
```c++
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
```
* Класс для хранения данных о сладостях. Есть конструкторы по умолчанию и с параметрами, метод print() для вывода информации.
## 3. Класс HashMap:
```c++
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
```
* Внутренняя структура KeyValuePair хранит ключ, значение и флаги. Вектор table — основное хранилище данных.
## 4. Хэш-функция Simple string hash:
```c++
    size_t hash(const std::string& key) const {
        size_t hashValue = 0;
        for (char c : key) {
            hashValue = hashValue * 31 + c; 
        }
        return hashValue % capacity;
    }
```
* Преобразует строку (ключ) в числовой индекс таблицы с помощью полиномиального хэширования.
## 5. Поиск индекса:
```c++
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
```
* Линейное пробирование для поиска свободного места или существующего ключа.
* Если ячейка свободна или помечена как удаленная - возвращаем индекс для вставки.
* Если ключ совпадает - возвращаем индекс для обновления/поиска. Иначе - продолжаем поиск.
## 6. Ресайз таблицы:
```c++ void resizeAndRehash() {
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
```
* Увеличивает размер таблицы в 2 раза при превышении load factor и перераспределяет элементы.
## 7.  Конструктор HashMap:
```c++
    HashMap(size_t initialCapacity = 10, float loadFactor = 0.7f)
        : capacity(initialCapacity), size(0), loadFactorThreshold(loadFactor) {
        table.resize(capacity);
    }
```
* Создаёт хеш-таблицу с начальной ёмкостью 10 и пороговым коэффициентом загрузки 0.7
## 8.  Метод insert() — добавление элемента:
```c++
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
```
## 9. Метод find() — поиск элемента:
```c++
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
```
## 10. Метод remove() — удаление элемента:
```c++
    bool remove(const std::string& key) {
        size_t index = findIndex(key);

        if (table[index].isOccupied && !table[index].isDeleted) {
            table[index].isDeleted = true; 
            size--;
            return true;
        }
        return false;
    }
```
## 11. Вывод содержимого:
```c++
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
```
## 12. Метод clear() — очистка таблицы:
```c++
    void clear() {
    for (size_t i = 0; i < capacity; ++i) {
        table[i].isOccupied = false;
        table[i].isDeleted = false;
    }
    size = 0; 
}
```
* Сбрасывает все флаги и обнуляет размер. Данные остаются в памяти, но помечаются как неиспользуемые.
## 13. Основные методы HashMap:
* insert() — добавляет или обновляет элемент.
* find() — ищет элемент по ключу.
* remove() — помечает элемент как удалённый.
* print() — выводит содержимое таблицы.
## 14. Пример использования в main():
```c++
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
```
## 15. Почему полиномиальное хэширование?
### Полиномиальное хеширование в этом коде удобно по нескольким причинам:
* Равномерное распределение ключей:
Полиномиальная функция (hashValue = hashValue * 31 + c) учитывает порядок символов и их значения, что минимизирует коллизии для строк с похожими символами.
* Учет всех символов строки:
Каждый символ вносит уникальный вклад в итоговый хеш. Это выгодно отличает метод от простого суммирования ASCII-кодов, где анаграммы (строки с одинаковыми символами в разном порядке) будут иметь одинаковый хеш.
## Скриншот из симулятора кастомных хэш-функций
![Снимок экрана (4)](https://github.com/user-attachments/assets/13e5ba54-f14a-4da3-b902-b2a398b3343b)

