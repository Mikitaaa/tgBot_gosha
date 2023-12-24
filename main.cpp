#include <stdio.h>
#include <tgbot/tgbot.h>
#include <vector>
#include <cstdlib>
#include <ctime>

#include "GPT2_handler.h"

int main() {
    srand(time(NULL));  // Seed the random number generator
    std::string model_path;
    std::cout << "Введите путь к модели GPT-2: ";
    std::cin >> model_path;
    std::cin.ignore(32767, '\n');
    
    std::vector<int> allowedChats = {-668390686};
    const int ResponseRate = 3;
    
    Gpt2Model model(model_path);
    TgBot::Bot bot("");
    
    int messageCount = 0;
    time_t lastUpdateTime = time(NULL);
    
bot.getEvents().onAnyMessage([&bot, &messageCount, &model, &allowedChats, &lastUpdateTime](TgBot::Message::Ptr message) {
    try {
        if (message->date < lastUpdateTime) { return; } // accept only new mwssages
                                
        messageCount++;
        auto it = std::find(allowedChats.begin(), allowedChats.end(), message->chat->id);
        if(it != allowedChats.end()){
            
            TgBot::ChatMember::Ptr member = bot.getApi().getChatMember(message->chat->id, bot.getApi().getMe()->id);
            if (member->status == "kicked" || member->status == "left") {
                allowedChats.erase(it);
                return; // do not handdle if bot not in a chat
            }

            if (!(messageCount % ResponseRate)) { // reply to every n'th message
                std::string response = model.generateResponse(message->text);
                bot.getApi().sendMessage(message->chat->id, response, true, message->messageId);
            }
        }else{
            bot.getApi().sendMessage(message->chat->id, "Извините, этот бот только для АВТОМАТИЗАЦИИ");
            bot.getApi().leaveChat(message->chat->id);
        }
                            
        lastUpdateTime = time(NULL);  // Update the last update time
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
});

    try {
        printf("Bot username: %s\n", bot.getApi().getMe()->username.c_str());
        TgBot::TgLongPoll longPoll(bot);
        
        while (true) {
            printf("Long poll started\n");
            longPoll.start();
        }
    } catch (TgBot::TgException& e) {
        printf("error: %s\n", e.what());
    }
    return 0;
}
