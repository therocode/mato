#include <fea/structure.hpp>
#include <fea/render2d.hpp>
#include <fea/util.hpp>
#include <fea/userinterface.hpp>
#include "inputhandler.hpp"
#include "messages.hpp"
#include "audioplayer.hpp"
#include "renderer.hpp"
#include "pixelmap.hpp"
#include "resourcemanager.hpp"
#include "object.hpp"

class Mato : public fea::Application,
    public fea::MessageReceiver<QuitMessage>
{
    public:
        Mato();
        void handleMessage(const QuitMessage& message) override;
        void addObject(Object object);
    protected:
        void loop() override;
    private:
        void createInitialData();
        void updateActionDurations(const std::vector<Action>& actions);
        void renderDisplays();
        fea::MessageBus mBus;
        fea::Window mWindow;
        fea::Renderer2D mFeaRenderer;
        fea::InputHandler mFeaInputHandler;

        //resources
        ResourceManager mResources;

        //modules
        InputHandler mInputHandler;
        AudioPlayer mAudioPlayer;
        Renderer mRenderer;
        
        //game data
        std::vector<Position> mPositions;
        std::vector<PlayerAim> mAims;
        std::vector<Health> mHealth;
        std::vector<ActionDuration> mActionDurations;
        std::vector<DisplayInstance> mDisplays;
        std::vector<AimDisplayInfo> mAimDisplayInfo;
        std::vector<WalkDisplayInfo> mWalkDisplayInfo;

        PixelMap mLandscapeForeground;
};
