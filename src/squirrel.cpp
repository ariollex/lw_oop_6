#include "squirrel.hpp"
#include "orc.hpp"
#include "bear.hpp"

Squirrel::Squirrel(const std::string &name, int x, int y) : NPC(SquirrelType, name, x, y) {}
Squirrel::Squirrel(std::istream &is) : NPC(SquirrelType, is) {}

void Squirrel::print()
{
    std::cout << *this;
}

void Squirrel::save(std::ostream &os)
{
    os << SquirrelType << std::endl;
    NPC::save(os);
}

FightResult Squirrel::fight(std::shared_ptr<Orc> other)
{
    fight_notify(other, false);
    return FightResult::None;
}

FightResult Squirrel::fight(std::shared_ptr<Squirrel> other)
{
    fight_notify(other, true);
    return FightResult::BothDead;
}

FightResult Squirrel::fight(std::shared_ptr<Bear> other)
{
    fight_notify(other, false);
    return FightResult::None;
}

FightResult Squirrel::accept(const std::shared_ptr<NPC> &attacker)
{
    return attacker->fight(std::static_pointer_cast<Squirrel>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Squirrel &squirrel)
{
    os << "squirrel: " << *static_cast<NPC *>(&squirrel) << std::endl;
    return os;
}
