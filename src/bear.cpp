#include "bear.hpp"
#include "squirrel.hpp"
#include "orc.hpp"

Bear::Bear(const std::string &name, int x, int y) : NPC(BearType, name, x, y) {}
Bear::Bear(std::istream &is) : NPC(BearType, is) {}

void Bear::print()
{
    std::cout << *this;
}

void Bear::save(std::ostream &os)
{
    os << BearType << std::endl;
    NPC::save(os);
}

FightResult Bear::fight(std::shared_ptr<Orc> other)
{
    fight_notify(other, false);
    return FightResult::None;
}

FightResult Bear::fight(std::shared_ptr<Squirrel> other)
{
    fight_notify(other, true);
    return FightResult::DefenderDead;
}

FightResult Bear::fight(std::shared_ptr<Bear> other)
{
    fight_notify(other, true);
    return FightResult::BothDead;
}

FightResult Bear::accept(const std::shared_ptr<NPC> &attacker)
{
    return attacker->fight(std::static_pointer_cast<Bear>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Bear &bear)
{
    os << "bear: " << *static_cast<NPC *>(&bear) << std::endl;
    return os;
}
