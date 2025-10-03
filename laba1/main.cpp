#include <random>
#include <iostream>
#include <iomanip>
#include <fstream>

class AbstractDice{
public:
    virtual ~AbstractDice() = default;
    virtual unsigned roll() = 0;
};


class Dice: public AbstractDice {
public:
    Dice(unsigned max, unsigned seed):
        max(max), dstr(1, max), reng(seed) { }
    unsigned roll() override {
        return dstr(reng);
    }
private:
    unsigned max;
    std::uniform_int_distribution<unsigned> dstr;
    std::default_random_engine reng;
};


class ThreeDicePool: public AbstractDice {
public:
    ThreeDicePool(Dice &dice1, Dice &dice2, Dice &dice3):
        d1(dice1), d2(dice2), d3(dice3) { }
    unsigned roll() override {
        return d1.roll() + d2.roll() + d3.roll();
    }
private:
    Dice &d1;
    Dice &d2;
    Dice &d3;
};





class PenaltyDice: public virtual AbstractDice {
public:
    PenaltyDice(AbstractDice &dice):
        d(dice) { }
    unsigned roll() override {
        unsigned roll1 = d.roll();
        unsigned roll2 = d.roll();
        return std::min(roll1, roll2);
    }
private:
    AbstractDice &d;
};


class BonusDice: public virtual AbstractDice {
public:
    BonusDice(AbstractDice &dice):
        d(dice) { }
    unsigned roll() override {
        unsigned roll1 = d.roll();
        unsigned roll2 = d.roll();
        return std::max(roll1, roll2);
    }
private:
    AbstractDice &d;
};





class DoubleDice: public PenaltyDice, public BonusDice {
public:
    DoubleDice(AbstractDice &dice):
        d(dice), PenaltyDice(dice), BonusDice(dice) { }
    unsigned roll() override {
        
        unsigned penalty_roll = PenaltyDice::roll();
        unsigned bonus_roll = BonusDice::roll();
        return (penalty_roll + bonus_roll) / 2;
    }
private:
    AbstractDice &d;
};


class DoubleDiceAlt: public AbstractDice {
public:
    DoubleDiceAlt(AbstractDice &dice):
        d(dice), penalty(dice), bonus(dice) { }
    
    unsigned roll() override {
        unsigned penalty_roll = penalty.roll();
        unsigned bonus_roll = bonus.roll();
        return (penalty_roll + bonus_roll) / 2;
    }
private:
    AbstractDice &d;
    PenaltyDice penalty;
    BonusDice bonus;
};







std::ofstream Dice_output("Dice.txt");

double expected_value(AbstractDice &ad, unsigned number_of_rolls = 1) {
    auto accum = 0llu;
    for (unsigned cnt = 0; cnt != number_of_rolls; ++cnt)
        accum += ad.roll();
    return
        static_cast<double>(accum) / static_cast<double>(number_of_rolls);
}

void roll_multiple_times(AbstractDice& ad, int times) {
        for (int i = 0; i < times; ++i) {
            Dice_output << "Roll " << (i + 1) << ": " << ad.roll() << std::endl;
        }
    }

double value_probability(unsigned value, AbstractDice &ad, unsigned number_of_rolls = 1) {
    unsigned count = 0;
    for (unsigned i = 0; i < number_of_rolls; ++i) {
        if (ad.roll() == value) {
            ++count;
        }
    }
    return static_cast<double>(count) / number_of_rolls;
}

void build_histogram(AbstractDice &ad, const std::string &title, 
                     unsigned min_value, unsigned max_value, unsigned bin,
                     unsigned number_of_rolls = 10000) {
    Dice_output << "\n" << title << std::endl;
    
    std::vector<double> probabilities;
    double total_prob = 0.0;
    
    for (unsigned value = min_value; value <= max_value; value += bin) {
        double prob = 0;
        for (unsigned i = value; i < value + bin; i++) {
            prob += value_probability(value, ad, number_of_rolls);
        }
        probabilities.push_back(prob);
        total_prob += prob;
        
        if (bin == 1) {
            Dice_output << std::setw(3) << value << ": " 
                  << std::fixed << std::setprecision(4) << prob << " ";
        }
        else {
            Dice_output << std::setw(3) << value << "-" << value + bin - 1 << ": " 
                  << std::fixed << std::setprecision(4) << prob << " ";
        }
               
                  
        int bars = static_cast<int>(prob * 500);
        for (int i = 0; i < bars; ++i) {
            Dice_output << "|";
        }
        Dice_output << std::endl;
    }
    
    Dice_output << "Сумма вероятностей: " << total_prob << std::endl;
}





int main() {
    unsigned rolls = 1000000;
    
    Dice_output << "Задание 1" << std::endl;
    //Dice
    Dice single_dice(6, 42);
    Dice_output << "\n1. Dice:" << std::endl;
    Dice_output << "Теоретическое мат. ожидание: " << (1 + 6) / 2.0 << std::endl;
    
    
    double exp_val_1 = expected_value(single_dice, rolls);
    Dice_output << "Экспериментальное мат. ожидание (" << rolls << " бросков): " 
              << std::fixed << std::setprecision(4) << exp_val_1 << std::endl;

    Dice_output << "Примеры бросков:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        Dice_output << single_dice.roll() << " ";
    }
    Dice_output << std::endl;
    
    //ThreeDicePool
    Dice d1(6, 42);
    Dice d2(6, 43);
    Dice d3(6, 44);
    ThreeDicePool three_dice(d1, d2, d3);
    Dice_output << "\n2. ThreeDicePool:" << std::endl;
    Dice_output << "Теоретическое мат. ожидание: " << 3 * (1 + 6) / 2.0 << std::endl;
    
    double exp_val_3 = expected_value(three_dice, rolls);
    Dice_output << "Экспериментальное мат. ожидание (" << rolls << " бросков): " 
              << exp_val_3 << std::endl;  
    
    Dice_output << "Примеры бросков:" << std::endl;
    for (int i = 0; i < 10; ++i) {
        Dice_output << three_dice.roll() << " ";
    }
    Dice_output << std::endl;


    //Полиморфизм типов
    roll_multiple_times(three_dice, 5);







    Dice_output << "\n\nЗадание 2" << std::endl;

    //Dice со значениями [1, 100]
    Dice_output << "\nОдин кубик [1, 100]" << std::endl;
    
    Dice single_dice_100(100, 42);
    PenaltyDice penalty_dice(single_dice_100);
    BonusDice bonus_dice(single_dice_100);
    
    Dice_output << "Мат. ожидание:" << std::endl;
    Dice_output << "Обычный бросок: " << expected_value(single_dice_100, rolls) << std::endl;
    Dice_output << "Со штрафом: " << expected_value(penalty_dice, rolls) << std::endl;
    Dice_output << "С преимуществом: " << expected_value(bonus_dice, rolls) << std::endl;
    
    build_histogram(single_dice_100, "Обычный бросок [1, 100]", 1, 100, 5, 200000);
    build_histogram(penalty_dice, "Бросок со штрафом [1, 100]", 1, 100, 5, 200000);
    build_histogram(bonus_dice, "Бросок с преимуществом [1, 100]", 1, 100, 5, 200000);




    //ThreeDicePool из трёх кубиков [1, 6]
    Dice_output << "\nThreeDicePool из трёх кубиков[1, 6]" << std::endl;
    
    PenaltyDice three_penalty(three_dice);
    BonusDice three_bonus(three_dice);
    
    Dice_output << "Мат. ожидание:" << std::endl;
    Dice_output << "Обычный бросок: " << expected_value(three_dice, rolls) << std::endl;
    Dice_output << "Со штрафом: " << expected_value(three_penalty, rolls) << std::endl;
    Dice_output << "С преимуществом: " << expected_value(three_bonus, rolls) << std::endl;
    
    build_histogram(three_dice, "ThreeDicePool - обычный бросок", 3, 18, 1, 20000);
    build_histogram(three_penalty, "ThreeDicePool - со штрафом", 3, 18, 1, 20000);
    build_histogram(three_bonus, "ThreeDicePool - с преимуществом", 3, 18, 1, 20000);


    Dice_output << "\n\nЗадание 3" << std::endl;

    Dice_output << "\nDoubleDice [1, 100]" << std::endl;


    DoubleDice double_dice(single_dice_100); 
    DoubleDiceAlt double_dice_alt(single_dice_100); 
    Dice_output << "Мат. ожидание DoubleDice: " << expected_value(double_dice, rolls) << std::endl;
    Dice_output << "Мат. ожидание DoubleDiceAlt:"  << expected_value(double_dice_alt, rolls) << std::endl;
    
    build_histogram(double_dice, "DoubleDice", 1, 100, 5, 20000);
    build_histogram(double_dice, "DoubleDiceAlt", 1, 100, 5, 20000);
    Dice_output.close();

    return 0;
}