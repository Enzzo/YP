#include "parser.h"

using namespace std;

namespace queries {

    //---------------------------- ComputeIncome ----------------------------
    class ComputeIncome : public ComputeQuery {
    public:
        using ComputeQuery::ComputeQuery;
        ReadResult Process(const BudgetManager& budget) const override {
            return { budget.ComputeSum(GetFrom(), GetTo()) };
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                return std::make_unique<ComputeIncome>(Date::FromString(parts[0]), Date::FromString(parts[1]));
            }
        };
    };

    //---------------------------- Alter ----------------------------
    class Alter : public ModifyQuery {
    public:
        Alter(Date from, Date to, double amount)
            : ModifyQuery(from, to)
            , amount_(amount) {
        }

        void Process(BudgetManager& budget) const override {
            double day_income = amount_ / (Date::ComputeDistance(GetFrom(), GetTo()) + 1);

            budget.AddBulkOperation(GetFrom(), GetTo(), BulkMoneyAdder{ day_income });
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double payload = std::stod(std::string(parts[2]));
                return std::make_unique<Alter>(Date::FromString(parts[0]), Date::FromString(parts[1]), payload);
            }
        };

    private:
        double amount_;
    };

    //---------------------------- PayTax ----------------------------
    class PayTax : public ModifyQuery {
    public:
        PayTax(Date from, Date to, const uint8_t amount)
            : ModifyQuery(from, to)
            , amount_(amount) {
        }

        void Process(BudgetManager& budget) const override {

            budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{ 1 });

        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double tax = std::stoi(std::string(parts[2]));
                return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]), tax);
            }
        };

    private:
        const uint8_t amount_;
    };

    //---------------------------- Spend ----------------------------
    class Spend : public ModifyQuery {
    public:
        Spend(Date from, Date to, const uint8_t amount)
            : ModifyQuery(from, to)
            , amount_(amount) {
        }

        void Process(BudgetManager& budget) const override {
            budget.AddBulkOperation(GetFrom(), GetTo(), BulkTaxApplier{ 1 });
        }

        class Factory : public QueryFactory {
        public:
            std::unique_ptr<Query> Construct(std::string_view config) const override {
                auto parts = Split(config, ' ');
                double spend = std::stoi(std::string(parts[2]));
                return std::make_unique<PayTax>(Date::FromString(parts[0]), Date::FromString(parts[1]), spend);
            }
        };

    private:
        const uint8_t amount_;
    };
}  // namespace queries

const QueryFactory& QueryFactory::GetFactory(std::string_view id) {
    static queries::ComputeIncome::Factory compute_income;
    static queries::Alter::Factory earn;
    static queries::PayTax::Factory pay_tax;
    static queries::Spend::Factory spend;
    static std::unordered_map<std::string_view, const QueryFactory&> factories
        = { {"ComputeIncome"sv, compute_income}, {"Earn"sv, earn}, {"PayTax"sv, pay_tax}, {"Spend"sv, spend} };

    return factories.at(id);
}
