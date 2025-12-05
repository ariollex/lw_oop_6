#pragma once

#include "npc.hpp"

struct Bear : public NPC
{
    Bear(const std::string &name, int x, int y);
    Bear(std::istream &is);

    void print() override;
    void save(std::ostream &os) override;
    FightResult fight(std::shared_ptr<Orc> other) override;
    FightResult fight(std::shared_ptr<Squirrel> other) override;
    FightResult fight(std::shared_ptr<Bear> other) override;
    FightResult accept(const std::shared_ptr<NPC> &attacker) override;
    friend std::ostream &operator<<(std::ostream &os, Bear &bear);
};
