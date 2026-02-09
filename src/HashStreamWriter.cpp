#include "HashStreamWriter.h"

void HashStreamWriter::addObserver(Observer* o) {
    Observable::addObserver(o);
    if (calculator) {
        calculator->addObserver(o);
    }
    reporter = dynamic_cast<ProgressReporter*>(o);///////////////////////////////////////////////////
}

void HashStreamWriter::setController(PauseController* c) {////////////////////////////////////////////////
    controller = c;
    if (calculator) calculator->setController(c);
}

ScanMemento HashStreamWriter::createMemento() {
    ScanMemento m;
    if (reporter) {
        reporter->saveToMemento(m);
    }
    return m;
}

void HashStreamWriter::processFile(File* file) {
    if (controller) controller->checkWait();/////////////////////////////////////

    notifyStart(file->getName());
    std::ifstream fileStream(file->getPath(), std::ios::binary);

    if (!fileStream.is_open()) {
        throw std::exception("File could not be opened");
    }
    std::string checksum = calculator->calculate(fileStream);

    notifyComplete(file->getName(), checksum);

    Visitor::os << getIndent() << "- " << file->getName() /*<< " [Checksum: " << checksum << "]" */ << std::endl;
}

void HashStreamWriter::processDir(Directory* dir)
{
    Visitor::os << getIndent() << "[" << dir->getName() << "]" << std::endl;
    ProcessingVisitor::processDir(dir);
}

HashStreamBuilder::HashStreamBuilder()
{
    VisitorBuilder::add(this->clone());
}

std::string HashStreamBuilder::getFLag() {
    return "--calculate";
}

void HashStreamBuilder::action(int, char* argv[], std::unique_ptr<ChecksumCalculator>& calc,
    std::unique_ptr<SystemItem>& root)
{
    if (calc == nullptr) {
        throw std::exception("Invalid argument flag");
    }
    std::cout << std::endl << "--- Mode: Checksum Calculation ---" << std::endl;

    HashStreamWriter hw(std::move(calc));

    ProgressReporter reporter(root->getSize());
    hw.addObserver(&reporter);

    PauseController ctr;
    hw.setController(&ctr);

    ScanMemento state;
    bool stateSaved = 0;

    std::thread worker(HashStreamBuilder::scanTask, root.get(), &hw);
    //root->accept(&hw);

    char input;
    std::cout << BLUE << "Controls: [p]ause, [r]esume, [s]ave state, [l]oad state, [q]uit" << RESET << std::endl;

    while (true) {
        std::cin >> input;

        if (input == 'p') {
            ctr.pause();
        }
        else if (input == 'r') {
            ctr.resume();
        }
        else if (input == 's') {
            ctr.pause();
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            state = hw.createMemento();
            stateSaved = true;
            std::cout << BLUE << "[System] State Saved." << RESET << std::endl;
        }
        else if (input == 'l') {
            if (stateSaved) {
                ctr.pause();
                hw.restoreMemento(state);
            }
            else {
                std::cout << BLUE << "[System] No saved state found." << RESET << std::endl;
            }
        }
        else if (input == 'q') {
            ctr.stop();
            if (worker.joinable()) {
                worker.detach();
            }
            break;
        }
    }

    if (worker.joinable()) worker.join();
}

std::unique_ptr<VisitorBuilder> HashStreamBuilder::clone() const {
    return std::make_unique<HashStreamBuilder>(*this);
}

void HashStreamBuilder::scanTask(SystemItem* root, HashStreamWriter* writer)
{
    try {
        root->accept(writer);
        std::cout << "\n--- Scan Complete ---" << std::endl;
    }
    catch (const std::exception& e) {
        std::cout << "\n[System] " << e.what() << std::endl;
    }
    std::cout << "Enter [q] to exit..." << std::endl;
}

static HashStreamBuilder __;