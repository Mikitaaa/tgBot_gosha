
#ifndef GPT2_handler_h
#define GPT2_handler_h

#include <string>

class Gpt2Model {
public:
    Gpt2Model(const std::string& modelPath);
    ~Gpt2Model() = default;

    std::string generateResponse(const std::string& inputText);

private:
    std::string model_path;
};

#endif /* GPT2_handler_h */
