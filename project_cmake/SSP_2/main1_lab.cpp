#include <iostream>
#include <vector>
#include <string>

class Component {
public:
    Component(const std::string& name, const std::string& brand, const std::string& model)
        : name(name), brand(brand), model(model) {}

    std::string GetInfo() const {
        return brand + " " + model + " (" + name + ")";
    }

private:
    std::string name = "";
    std::string brand = "";
    std::string model = "";
};

class Processor : public Component {
public:
    Processor(const std::string& brand, const std::string& model, float frequency, int cores)
        : Component("Processor", brand, model), frequency(frequency), cores(cores) {}

    float GetFrequency() const {
        return frequency;
    }

    std::string GetInfo() const {
        return Component::GetInfo() + " - " + std::to_string(frequency) + " GHz, " + std::to_string(cores) + " cores";
    }

private:
    float frequency = 0.0f;
    int cores = 0;
};

class Memory : public Component {
public:
    Memory(const std::string& brand, const std::string& model, float frequency, int capacity)
        : Component("Memory", brand, model), frequency(frequency), capacity(capacity) {}

    std::string GetInfo() const {
        return Component::GetInfo() + " - " + std::to_string(frequency) + " GHz, " + std::to_string(capacity) + " GB";
    }

private:
    float frequency = 0.0f;
    int capacity = 0;
};

class Storage : public Component {
public:
    Storage(const std::string& brand, const std::string& model, int capacity)
        : Component("Storage", brand, model), capacity(capacity) {}

    std::string GetInfo() const {
        return Component::GetInfo() + " - " + std::to_string(capacity) + " GB";
    }

private:
    int capacity = 0;
};

class PC {
public:
    PC(const Processor& processor, const Memory& memory, const Storage& storage)
        : processor(processor), memory(memory), storage(storage) {}

    float GetProcessorFrequency() const {
        return processor.GetFrequency();
    }

    std::string GetProcessorData() const {
        return processor.GetInfo();
    }

private:
    Processor processor;
    Memory memory;
    Storage storage;
};

int main() {
    Processor processor1("Intel", "i5-9600K", 3.7, 6);
    Memory memory1("Corsair", "CMK16GX4M2B3200C16", 3.2, 16);
    Storage storage1("Samsung", "MZ-V7E500BW", 500);

    Processor processor2("AMD", "Ryzen 7 3700X", 3.6, 8);
    Memory memory2("G.Skill", "F4-3200C16D-16GVKB", 3.2, 32);
    Storage storage2("Crucial", "CT500MX500SSD1", 1000);

    PC pc1(processor1, memory1, storage1);
    PC pc2(processor2, memory2, storage2);

    std::cout << "PC with the highest processor frequency:\n";
    std::cout << ((pc1.GetProcessorFrequency() > pc2.GetProcessorFrequency()) ? pc1 : pc2).GetProcessorData() << std::endl;

    return 0;
}
