#include <catch2/catch_test_macros.hpp>
#include <string>
#include <vector>
#include <typeinfo>

#include "ReportWriter.h"
#include "ChecksumCalculator.h"
#include "SystemItem.h"

#include "XML_RW.h"     
#include "Default_RW.h" 


class MockCalculator : public ChecksumCalculator {
public:
    std::string calculate(std::istream&) const override { return ""; }
};

class SpySystemItem : public SystemItem {
public:
    bool visitedByXML = false;
    bool visitedByDefault = false;

    SpySystemItem() : SystemItem("root", "root") {}

    void accept(Visitor* v) override {
        if (dynamic_cast<XML_RW*>(v) != nullptr) {
            visitedByXML = true;
        }
        else if (dynamic_cast<Default_RW*>(v) != nullptr) {
            visitedByDefault = true;
        }
    }

    size_t getSize() const override { return 0; }
};


TEST_CASE("ReportBuilder: Configuration") {
    ReportBuilder builder;

    SECTION("Flag Check") {
        REQUIRE(builder.getFLag() == "--report");
    }

    SECTION("Clone Check") {
        std::unique_ptr<VisitorBuilder> clone = builder.clone();
        REQUIRE(clone != nullptr);

        ReportBuilder* derived = dynamic_cast<ReportBuilder*>(clone.get());

        REQUIRE(derived != nullptr);
        REQUIRE(derived->getFLag() == "--report");
    }
}

TEST_CASE("ReportBuilder: Argument Parsing & dispatch") {
    ReportBuilder builder;
    auto calc = std::unique_ptr<ChecksumCalculator>(new MockCalculator());

    auto rootSpy = std::make_unique<SpySystemItem>();

    SpySystemItem* spyRef = rootSpy.get();

    std::unique_ptr<SystemItem> rootBase = std::move(rootSpy);

    SECTION("Throws exception if Calculator is null") {
        std::unique_ptr<ChecksumCalculator> nullCalc = nullptr;

        char* argv[] = { (char*)"exe", (char*)"path", (char*)"--report" };

        REQUIRE_THROWS_AS(
            builder.action(3, argv, nullCalc, rootBase),
            std::exception
        );
    }

    SECTION("Creates Default_RW when no extra flag provided") {
        char* argv[] = { (char*)"exe", (char*)"path", (char*)"--report" };
        int argc = 3;

        builder.action(argc, argv, calc, rootBase);

        REQUIRE(spyRef->visitedByDefault == true);
        REQUIRE(spyRef->visitedByXML == false);
    }

    SECTION("Creates XML_RW when '-x' flag is provided") {
        char* argv[] = { (char*)"exe", (char*)"path", (char*)"--report", (char*)"-x" };
        int argc = 4;

        builder.action(argc, argv, calc, rootBase);

        REQUIRE(spyRef->visitedByXML == true);
        REQUIRE(spyRef->visitedByDefault == false);
    }
}