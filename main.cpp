#include "npc.hpp"
#include "orc.hpp"
#include "squirrel.hpp"
#include "bear.hpp"
#include <ctime>
#include <cstdlib>

// Text Observer
class TextObserver : public IFightObserver
{
private:
    TextObserver(){};

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static TextObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (!win)
            return;
        std::cout << std::endl
                  << "Murder --------" << std::endl;
        attacker->print();
        defender->print();
    }
};

// File Observer
class FileObserver : public IFightObserver
{
private:
    std::ofstream out;
    FileObserver() : out("log.txt", std::ios::out | std::ios::trunc) {}

public:
    static std::shared_ptr<IFightObserver> get()
    {
        static FileObserver instance;
        return std::shared_ptr<IFightObserver>(&instance, [](IFightObserver *) {});
    }

    void on_fight(const std::shared_ptr<NPC> attacker, const std::shared_ptr<NPC> defender, bool win) override
    {
        if (!win || !out.is_open())
            return;
        out << "Murder: " << attacker->name << " -> " << defender->name << " (" << defender->x << "," << defender->y << ")" << std::endl;
        out.flush();
    }
};

// Фабрики -----------------------------------
std::shared_ptr<NPC> factory(std::istream &is)
{
    std::shared_ptr<NPC> result;
    int type{0};
    if (is >> type)
    {
        switch (type)
        {
        case OrcType:
            result = std::make_shared<Orc>(is);
            break;
        case SquirrelType:
            result = std::make_shared<Squirrel>(is);
            break;
        case BearType:
            result = std::make_shared<Bear>(is);
            break;
        }
    }
    else
        std::cerr << "unexpected NPC type:" << type << std::endl;

    if (result)
    {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

std::shared_ptr<NPC> factory(NpcType type, const std::string &name, int x, int y)
{
    std::shared_ptr<NPC> result;
    switch (type)
    {
    case OrcType:
        result = std::make_shared<Orc>(name, x, y);
        break;
    case SquirrelType:
        result = std::make_shared<Squirrel>(name, x, y);
        break;
    case BearType:
        result = std::make_shared<Bear>(name, x, y);
        break;
    default:
        break;
    }
    if (result)
    {
        result->subscribe(TextObserver::get());
        result->subscribe(FileObserver::get());
    }

    return result;
}

// save array to file
void save(const set_t &array, const std::string &filename)
{
    std::ofstream fs(filename);
    fs << array.size() << std::endl;
    for (auto &n : array)
        n->save(fs);
    fs.flush();
    fs.close();
}

set_t load(const std::string &filename)
{
    set_t result;
    std::ifstream is(filename);
    if (is.good() && is.is_open())
    {
        int count;
        is >> count;
        for (int i = 0; i < count; ++i)
            result.insert(factory(is));
        is.close();
    }
    else
    {
        std::cerr << "Error: " << std::strerror(errno) << std::endl;
        std::exit(EXIT_FAILURE);
    }
    return result;
}

// print to screen
std::ostream &operator<<(std::ostream &os, const set_t &array)
{
    for (auto &n : array)
        n->print();
    return os;
}

set_t fight(const set_t &array, size_t distance)
{
    set_t dead_list;

    for (const auto &attacker : array)
        for (const auto &defender : array)
            if ((attacker != defender) && (attacker->is_close(defender, distance)))
            {
                const FightResult outcome = defender->accept(attacker);
                if (outcome == FightResult::DefenderDead || outcome == FightResult::BothDead)
                    dead_list.insert(defender);
                if (outcome == FightResult::BothDead)
                    dead_list.insert(attacker);
            }

    return dead_list;
}

int main(int argc, char **argv)
{
    set_t array; // монстры
    size_t orc_count = 0, squirrel_count = 0, bear_count = 0;

    std::string filename = (argc > 1) ? argv[1] : "npc.txt";
    const bool generate = (argc <= 1);

    std::srand(static_cast<unsigned>(std::time(nullptr)));

    const int MAX_X{500};
    const int MAX_Y{500};

    auto next_name = [&](NpcType t) {
        switch (t)
        {
        case OrcType:
            return std::string("Orc_") + std::to_string(++orc_count);
        case SquirrelType:
            return std::string("Squirrel_") + std::to_string(++squirrel_count);
        case BearType:
            return std::string("Bear_") + std::to_string(++bear_count);
        default:
            return std::string("NPC_") + std::to_string(orc_count + squirrel_count + bear_count + 1);
        }
    };

    if (generate)
    // Генерируем начальное распределение монстров
    {
        std::cout << "Generating ..." << std::endl;
        for (size_t i = 0; i < 100; ++i)
        {
            auto type = NpcType(std::rand() % 3 + 1);
            array.insert(factory(type,
                                 next_name(type),
                                 std::rand() % (MAX_X + 1),
                                 std::rand() % (MAX_Y + 1)));
        }
        std::cout << "Saving ..." << std::endl;
        save(array, filename);
    }

    std::cout << "Loading ..." << std::endl;
    array.clear();
    array = load(filename);

    std::cout << "Fighting ..." << std::endl
              << array;

    for (size_t distance = 20; (distance <= 100) && !array.empty(); distance += 10)
    {
        auto dead_list = fight(array, distance);
        for (auto &d : dead_list) array.erase(d);
        std::cout << "Fight stats ----------" << std::endl
                  << "distance: " << distance << std::endl
                  << "killed: " << dead_list.size() << std::endl
                  << std::endl << std::endl;
    }

    std::cout << "Survivors:" << array;

    return 0;
}
