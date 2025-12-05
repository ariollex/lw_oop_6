#pragma once

#include <iostream>
#include <memory>
#include <cstring>
#include <string>
#include <random>
#include <fstream>
#include <set>
#include <vector>
#include <math.h>
#include <stdexcept>
#include <algorithm>

// type for npcs
struct NPC;
struct Orc;
struct Squirrel;
struct Bear;
using set_t = std::set<std::shared_ptr<NPC>>;

enum class FightResult
{
    None,
    DefenderDead,
    BothDead
};

enum NpcType
{
    Unknown = 0,
    OrcType = 1,
    SquirrelType = 2,
    BearType = 3
};

struct IFightObserver{
    virtual void on_fight(const std::shared_ptr<NPC> attacker,const std::shared_ptr<NPC> defender,bool win) = 0;
};

struct NPC : public std::enable_shared_from_this<NPC>
{
    static constexpr int MIN_COORD{0};
    static constexpr int MAX_COORD{500};
    NpcType type;
    std::string name;
    int x{0};
    int y{0};
    std::vector<std::shared_ptr<IFightObserver>> observers;
    static std::set<std::string> used_names;

    NPC(NpcType t, const std::string &_name, int _x, int _y);
    NPC(NpcType t, std::istream &is);
    virtual ~NPC();

    void subscribe(std::shared_ptr<IFightObserver>observer );
    void fight_notify(const std::shared_ptr<NPC> defender,bool win);
    virtual bool is_close(const std::shared_ptr<NPC> &other, size_t distance) const;

    virtual FightResult fight(std::shared_ptr<Orc> other) = 0;
    virtual FightResult fight(std::shared_ptr<Squirrel> other) = 0;
    virtual FightResult fight(std::shared_ptr<Bear> other) = 0;
    virtual FightResult accept(const std::shared_ptr<NPC> &attacker) = 0;
    virtual void print() = 0;

    virtual void save(std::ostream &os);

    friend std::ostream &operator<<(std::ostream &os, NPC &npc);
};
