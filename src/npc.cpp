#include "npc.hpp"

std::set<std::string> NPC::used_names{};

NPC::NPC(NpcType t, const std::string &_name, int _x, int _y) : type(t), name(_name), x(std::clamp(_x, MIN_COORD, MAX_COORD)), y(std::clamp(_y, MIN_COORD, MAX_COORD))
{
    if (!used_names.insert(name).second)
        throw std::invalid_argument("NPC name must be unique");
}

NPC::NPC(NpcType t, std::istream &is) : type(t)
{
    is >> name;
    is >> x;
    is >> y;
    x = std::clamp(x, MIN_COORD, MAX_COORD);
    y = std::clamp(y, MIN_COORD, MAX_COORD);
    if (!used_names.insert(name).second)
        throw std::invalid_argument("NPC name must be unique");
}

NPC::~NPC()
{
    used_names.erase(name);
}

void NPC::subscribe(std::shared_ptr<IFightObserver> observer)
{
   observers.push_back(observer);
}

void NPC::fight_notify(const std::shared_ptr<NPC> defender, bool win)
{
    for (auto &o : observers)
        o->on_fight(shared_from_this(), defender, win);
}

bool NPC::is_close(const std::shared_ptr<NPC> &other, size_t distance) const
{
    if (std::pow(x - other->x, 2) + std::pow(y - other->y, 2) <= std::pow(distance, 2))
        return true;
    else
        return false;
}

void NPC::save(std::ostream &os)
{
    os << name << std::endl;
    os << x << std::endl;
    os << y << std::endl;
}

std::ostream &operator<<(std::ostream &os, NPC &npc)
{
    os << "{ name:" << npc.name << ", x:" << npc.x << ", y:" << npc.y << "} ";
    return os;
}