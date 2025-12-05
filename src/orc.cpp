#include "orc.hpp"
#include "bear.hpp"
#include "squirrel.hpp"

Orc::Orc(const std::string &name, int x, int y) : NPC(OrcType, name, x, y) {}
Orc::Orc(std::istream &is) : NPC(OrcType, is) {}

void Orc::print()
{
    std::cout << *this;
}

void Orc::save(std::ostream &os)
{
    os << OrcType << std::endl;
    NPC::save(os);
}

FightResult Orc::fight(std::shared_ptr<Orc> other)
{
    fight_notify(other, true);
    return FightResult::BothDead;
}

FightResult Orc::fight(std::shared_ptr<Squirrel> other)
{
    fight_notify(other, false);
    return FightResult::None;
}

FightResult Orc::fight(std::shared_ptr<Bear> other)
{
    fight_notify(other, true);
    return FightResult::DefenderDead;
}

FightResult Orc::accept(const std::shared_ptr<NPC> &attacker)
{
    return attacker->fight(std::static_pointer_cast<Orc>(shared_from_this()));
}

std::ostream &operator<<(std::ostream &os, Orc &orc)
{
    os << "orc: " << *static_cast<NPC *>(&orc) << std::endl;
    return os;
}
