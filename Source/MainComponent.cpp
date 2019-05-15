/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#include "MainComponent.h"

//==============================================================================
MainComponent::MainComponent() : engine("Ok engine")
{
    addAndMakeVisible(playButton);
    // Make sure you set the size of the component after
    // you add any child components.
    setSize (400, 400);

    // Some platforms require permissions to open input channels so request that here
    if (RuntimePermissions::isRequired (RuntimePermissions::recordAudio)
        && ! RuntimePermissions::isGranted (RuntimePermissions::recordAudio))
    {
        RuntimePermissions::request (RuntimePermissions::recordAudio,
                                     [&] (bool granted) { if (granted)  setAudioChannels (2, 2); });
    }
    else
    {
        // Specify the number of input and output channels that we want to open
        setAudioChannels (2, 2);
    }
    
    DBG(juce::File::getCurrentWorkingDirectory().getFullPathName());
    te::Edit::LoadContext loadContext;
    edit = std::make_unique<te::Edit>(engine,
                                     te::createEmptyEdit(),
                                     te::Edit::forEditing,
                                     &loadContext,
                                     0);
    auto editFile = juce::File::getCurrentWorkingDirectory().getChildFile("default.tracktionedit");
    auto audioFile = juce::File("C:\\projects-charles\\reaper\\mw-spring-2019.wav");
    if (audioFile.existsAsFile())
        te::getFirstAudioTrack(*edit)->insertWaveClip("Charles Wave Clip", audioFile, { { 0., 60. }, 0. }, false);
    else
        DBG("File Not Found: " + audioFile.getFullPathName());

    edit->editFileRetriever = [editFile] { return editFile; };
    // Enable looping
    edit->getTransport().play(false);
    edit->getTransport().setLoopRange({ 0, 20 });
    edit->getTransport().looping = true;

    // Start playback on button click
    playButton.onClick = [this] {
        if (edit != nullptr) {
            edit->getTransport().play(false);
            DBG("Playing...");
        }
    };
}

MainComponent::~MainComponent()
{
    // This shuts down the audio device and clears the audio source.
    if (auto e = edit.get()) {
        DBG("SAVE: ");
        DBG(e->editFileRetriever().getFullPathName());
        te::EditFileOperations(*e).save(true, true, false);
    }
    shutdownAudio();
    
}

//==============================================================================
void MainComponent::prepareToPlay (int samplesPerBlockExpected, double sampleRate)
{
    // This function will be called when the audio device is started, or when
    // its settings (i.e. sample rate, block size, etc) are changed.

    // You can use this function to initialise any resources you might need,
    // but be careful - it will be called on the audio thread, not the GUI thread.

    // For more details, see the help for AudioProcessor::prepareToPlay()
}

void MainComponent::getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill)
{
    // Your audio-processing code goes here!

    // For more details, see the help for AudioProcessor::getNextAudioBlock()

    // Right now we are not producing any data, in which case we need to clear the buffer
    // (to prevent the output of random noise)
    bufferToFill.clearActiveBufferRegion();
}

void MainComponent::releaseResources()
{
    // This will be called when the audio device stops, or when it is being
    // restarted due to a setting change.

    // For more details, see the help for AudioProcessor::releaseResources()
}

//==============================================================================
void MainComponent::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.setColour(getLookAndFeel().findColour(TextPropertyComponent::textColourId));
    if (edit) {
        g.drawText(String(edit->getTransport().getCurrentPosition()), 20, 40, 100, 40, Justification::centred, true);
        g.drawText(String("Playing? ") + (edit->getTransport().isPlaying() ? String{ "true" } : String{ "false" }), 20, 60, 100, 40, Justification::centred, true);
    }

}

void MainComponent::resized()
{
    currentSizeAsString = String(getWidth()) + "x" + String(getHeight());
    playButton.setBounds(10, 10, 80, 20);
}
