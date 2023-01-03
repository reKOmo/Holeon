#include "DialogManager.h"
#include "ButtonManager.h"

Engine::Entity createDialog(Engine::Scene* scene) {
	auto dialogContainre = scene->createEntity("dialogContainer");
	auto& containerTrans = dialogContainre.addComponent<Engine::Components::TransformComponent>();
	raylib::Vector2 containerSize = { 450.0, 150.0 };
	containerTrans.Position = { (float)640.0 - (float)(containerSize.x / 2.0), 500.0};
	Engine::Renderer::Material dialogBg = { scene->m_TextureManager->getTexture(4), {0.0, 0.0, 12.0, 12.0} };
	auto& containerBg = dialogContainre.addComponent<Engine::Components::BackgroundComponent>(dialogBg);
	containerBg.scale = { 4.0, 4.0 };
	containerBg.size = containerSize;

	auto dialogText = scene->createEntity("dialogText");
	auto& textTrans = dialogText.addComponent<Engine::Components::TransformComponent>();
	textTrans.Position = { 20.0, 20.0 };
	dialogText.setParent(dialogContainre);
	auto& text = dialogText.addComponent<Engine::Components::TextComponent>();
	text.font = scene->m_FontManager->getFont(0);
	text.fontSize = 24;
	text.spacing = 2;

	dialogContainre.disable();


	return dialogContainre;
}

Engine::Entity createPickContainer(Engine::Scene* scene) {
	auto battleUI = scene->createEntity("optionsList");
	auto& trans = battleUI.addComponent<Engine::Components::TransformComponent>();
	trans.Position = { 0.0, -150.0 };
	battleUI.setParent(scene->getEntityByName("dialogContainer"));
	battleUI.addComponent<Engine::Components::ScriptComponent>().bind<ButtonManager>();
	auto& uiMgrData = battleUI.addComponent<Engine::Components::ButtonManagerData>();

	return battleUI;
}

Engine::Entity creaateDialogButton(Engine::Scene* scene) {
	raylib::Font* f = scene->m_FontManager->getFont(0);
	const int fontSize = 24;
	const int spacing = 2;
	const float textOffset = 20;



	auto uiBg = scene->createEntity();
	auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
	uiTrans.Position = { 0.0, 0.0 };
	Engine::Renderer::Material dialogFrame = { scene->m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
	auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
	bg.scale = { 4.0, 4.0 };

	auto uiText = scene->createEntity();
	auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
	uiTextTrans.Position = { 20.0, 20.0 };
	uiText.setParent(uiBg);

	auto& textComp = uiText.addComponent<Engine::Components::TextComponent>();
	textComp.fontSize = fontSize;
	textComp.spacing = spacing;
	textComp.font = f;
	textComp.color = { 100, 100, 100, 255 };

	uiBg.addComponent<Engine::Components::ButtonComponent>();

	return uiBg;
}

void DialogManager::onCreate() {
	dialogContainer = Instattiate(createDialog);
}

void DialogManager::onUpdate(float delta) {
	if (endInteraction) {
		isDisplaying = false;
		dialogContainer.disable();
		onEnd();
		endInteraction = false;
		unpauseGame();
		optionsEnt = Engine::Entity();
		onEndQuestion(pickedValue);
		optionsSelectOn = false;
	}
	if (isDisplaying) {
		if (IsKeyPressed(KEY_ENTER)) {
			endInteraction = true;
		}
	}
	if (optionsEnt && !optionsSelectOn) {
		//set cllbacks
		auto& script = optionsEnt.getComponent<Engine::Components::ScriptComponent>();
		if (script) {
			auto buttonMgr = dynamic_cast<ButtonManager*>(script.instance);
			buttonMgr->onSelect([&](int a) {
				endInteraction = true;
				pickedValue = a;
			});
			optionsSelectOn = true;
		}
	}
}

void DialogManager::showDialog(std::string msg, std::function<void()> onEnd) {
	resetCallbacks();
	isDisplaying = true;
	dialogContainer.enable();
	this->onEnd = onEnd;
	auto& children = dialogContainer.getChildren();
	children[0].getComponent<Engine::Components::TextComponent>().text = msg;
	pauseGame();
}

void DialogManager::showDialog(std::vector<std::string>& msg, std::function<void()> onEnd) {
}

void DialogManager::pickDialog(std::string question, std::string yes, std::string no, std::function<void(bool)> onEnd) {
	resetCallbacks();
	dialogContainer.enable();
	onEndQuestion = onEnd;
	auto& children = dialogContainer.getChildren();
	children[0].getComponent<Engine::Components::TextComponent>().text = question;
	pauseGame();

	// create buttons
	auto optionsContainer = Instattiate(createPickContainer);
	
	auto button1 = Instattiate(creaateDialogButton);
	button1.setParent(optionsContainer);
	auto button2 = Instattiate(creaateDialogButton);
	button2.setParent(optionsContainer);

	optionsContainer.getComponent<Engine::Components::ButtonManagerData>().currentButton = button1;

	auto& btn1Comp = button1.getComponent<Engine::Components::ButtonComponent>();
	btn1Comp.value = 1;
	btn1Comp.bottom = button2;
	auto& btn1Children = button1.getChildren();
	auto& btn1Txt = btn1Children[0].getComponent<Engine::Components::TextComponent>();
	btn1Txt.text = yes;
	auto& btn1Bg = button1.getComponent<Engine::Components::BackgroundComponent>();
	auto textSize1 = btn1Txt.font->MeasureText(yes, 24, 2);
	btn1Bg.size = { textSize1.x + 40, textSize1.y + 40 };

	auto& btn2Comp = button2.getComponent<Engine::Components::ButtonComponent>();
	btn2Comp.value = 0;
	btn2Comp.top = button1;
	auto& btn2Children = button2.getChildren();
	auto& btn2Txt = btn2Children[0].getComponent<Engine::Components::TextComponent>();
	btn2Txt.text = no;
	auto& btn2Bg = button2.getComponent<Engine::Components::BackgroundComponent>();
	auto textSize2 = btn2Txt.font->MeasureText(no, 24, 2);
	btn2Bg.size = { textSize2.x + 40, textSize2.y + 40 };
	auto& btn2Trans = button2.getComponent<Engine::Components::TransformComponent>();
	btn2Trans.Position.y = textSize2.y + 45;

	optionsEnt = optionsContainer;
}

void DialogManager::pauseGame() {
	auto toPause = getEntitiesByTag("pauseable");
	for (auto& ent : toPause) {
		ent.getComponent<Engine::Components::ScriptComponent>().paused = true;
	}
	getSceneManager().getActveScene().setTimeScale(0.0);
}
void DialogManager::unpauseGame() {
	auto toPause = getEntitiesByTag("pauseable");
	for (auto& ent : toPause) {
		ent.getComponent<Engine::Components::ScriptComponent>().paused = false;
	}
	getSceneManager().getActveScene().setTimeScale(1.0);
}

void DialogManager::resetCallbacks() {
	onEnd = []() {};
	onEndQuestion = [](bool a) {};
}
