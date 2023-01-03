#include "BattlePlayerUI.h"
#include "GameUtils.h"
#include "ButtonManager.h"
#include "Inventory.h"

Engine::Entity createWeaponUi(Engine::Scene* scene) {
	auto cont = scene->createEntity("weaponSwitchMenu");
	auto& attackMenuTras = cont.addComponent<Engine::Components::TransformComponent>();
	attackMenuTras.Position = { 850.0, 160.0 };

	raylib::Font* f = scene->m_FontManager->getFont(0);
	const int fontSize = 24;
	const int spacing = 2;
	const float textOffset = 20;

	std::string text = "Weapons";

	auto uiBg = scene->createEntity();
	auto& uiTrans = uiBg.addComponent<Engine::Components::TransformComponent>();
	uiTrans.Position = { 90.0, 0.0 };
	Engine::Renderer::Material dialogFrame = { scene->m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
	auto& bg = uiBg.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
	bg.scale = { 4.0, 4.0 };
	auto textSize = f->MeasureText(text, fontSize, spacing);
	bg.size = { textSize.x + 40.0f, textSize.y + 40.0f };
	uiBg.setParent(cont);

	auto uiText = scene->createEntity();
	auto& uiTextTrans = uiText.addComponent<Engine::Components::TransformComponent>();
	uiTextTrans.Position = { 20.0, 20.0 };
	uiText.setParent(uiBg);

	auto& textComp = uiText.addComponent<Engine::Components::TextComponent>();
	textComp.fontSize = fontSize;
	textComp.spacing = spacing;
	textComp.font = f;
	textComp.color = BLACK;
	textComp.text = text;


	return cont;
}

Engine::Entity createItemsUI(Engine::Scene* scene) {
	auto cont = scene->createEntity("itemMenu");
	auto& attackMenuTras = cont.addComponent<Engine::Components::TransformComponent>();
	attackMenuTras.Position = { 850.0, 160.0 };

	return cont;
}

Engine::Entity createAttacksUI(Engine::Scene* scene) {
	Engine::Renderer::Material dialogFrame = { scene->m_TextureManager->getTexture(4), { 0.0, 0.0, 12.0, 12.0 } };
	// attack menu
	auto attackMenu = scene->createEntity("attackMenu");
	auto& attackMenuTras = attackMenu.addComponent<Engine::Components::TransformComponent>();
	attackMenuTras.Position = { 850.0, 160.0 };

	auto weapnShow = scene->createEntity("weaponShow");
	auto& waeponShowTrans = weapnShow.addComponent<Engine::Components::TransformComponent>();
	weapnShow.setParent(attackMenu);
	auto& weaponBg = weapnShow.addComponent<Engine::Components::BackgroundComponent>(dialogFrame);
	weaponBg.size = { 200, 250 };
	weaponBg.scale = { 4.0, 4.0 };


	auto weapon = scene->createEntity("weaponSprite");
	weapon.setParent(weapnShow);
	auto& weponSpriteImage = weapon.addComponent<Engine::Components::ImageComponent>();
	weponSpriteImage.scale = { 5.0, 5.0 };
	auto& weaponTrans = weapon.addComponent<Engine::Components::TransformComponent>();
	weaponTrans.Position = { 20.0, 45 };

	auto attacksList = scene->createEntity("attackList");
	attacksList.setParent(attackMenu);
	auto& attackListTrans = attacksList.addComponent<Engine::Components::TransformComponent>();
	attackListTrans.Position = { 230.0, 0.0 };

	return attackMenu;
}


void BattleUIPlayer::onCreate() {
	playerStats = getGlobalStorage().get<EntityStats>("playerStats");

	/*
		=================
			Attacks UI
		=================
	*/
	attackUI = Instattiate(createAttacksUI);

	Instattiate([&](Engine::Scene* scene) {
		Engine::Entity buttons[Attack::maxAttacks];

		auto battleMenu = scene->getEntityByName("attackMenu");

		battleMenu.addComponent<Engine::Components::ScriptComponent>().bind<ButtonManager>();
		auto& uiMgrData = battleMenu.addComponent<Engine::Components::ButtonManagerData>();


		auto battleUI = scene->getEntityByName("attackList");

		auto& weapon = playerStats->currentWeapon;
		for (int i = 0; i < Attack::maxAttacks; i++) {
			if (weapon.attacks[i]) {
				auto button = createDialogButton(weapon.attacks[i].name, "button", i, scene);
				button.getComponent<Engine::Components::BackgroundComponent>().scale = { 4.0, 4.0 };
				if (i > 0) {
					button.getComponent<Engine::Components::ButtonComponent>().top = buttons[i - 1];
					buttons[i - 1].getComponent<Engine::Components::ButtonComponent>().bottom = button;

					auto& buttonTrans = button.getComponent<Engine::Components::TransformComponent>();
					buttonTrans.Position = { 0.0, (float)i * 70.0f };
				}
				button.setParent(battleUI);
				if (i == 0) {
					uiMgrData.currentButton = button;
				}
				buttons[i] = button;
			}
		}

		return battleUI;
	});
	auto weaponSprite = getEntityByName("weaponSprite");
	weaponSprite.getComponent<Engine::Components::ImageComponent>().material = { getSceneManager().getActveScene().m_TextureManager->getTexture(0), {0.0, 0.0, 32.0, 32.0} };
	attackUI.disable();

	/*
		=================
			Weapon UI
		=================
	*/
	weaponsUI = Instattiate(createWeaponUi);
	weaponsUI.disable();


	/*
		=================
			Item UI
		=================
	*/

	itemsUI = Instattiate(createItemsUI);
	Instattiate([&](Engine::Scene* scene) {

		auto battleUI = scene->getEntityByName("itemMenu");


		battleUI.addComponent<Engine::Components::ScriptComponent>().bind<ButtonManager>();
		auto& uiMgrData = battleUI.addComponent<Engine::Components::ButtonManagerData>();

		auto button = createDialogButton("aaaaaaaaaaaa", "button", 0, scene);
		button.getComponent<Engine::Components::BackgroundComponent>().scale = { 4.0, 4.0 };

		uiMgrData.currentButton = button;

		button.setParent(battleUI);


		return battleUI;
	});
	itemsUI.disable();

	currentMenu = attackUI;
}

void BattleUIPlayer::onUpdate(float delta) {
	if (!isSetup) {
		auto& script = currentMenu.getComponent<Engine::Components::ScriptComponent>();
		if (script) {
			auto mgr = dynamic_cast<ButtonManager*>(script.instance);
			mgr->onSelect([&](int val) {
				onSelect(val);
			});
			isSetup = true;
		}
	}

	if (isShown) {
		bool switched = false;
		if (IsKeyPressed(KEY_A)) {
			currentMenuId = std::abs(currentMenuId - 1) % 3;
			switched = true;
		}
		else if (IsKeyPressed(KEY_D)) {
			currentMenuId = (currentMenuId + 1) % 3;
			switched = true;
		}

		if (switched) {
			currentMenu.disable();
			if (currentMenu == weaponsUI) {
				// delete buttons
				auto children = weaponsUI.getChildren();
				for (int i = 1; i < children.size(); i++) {
					auto& ent = children[i];
					getSceneManager().getActveScene().removeEntity(ent);
					weaponsUI.removeChild(ent);
				}
			}
			
			switch (currentMenuId) {
			case 0:
				currentMenu = attackUI;
				break;
			case 1:
				currentMenu = weaponsUI;
				createWeaponButtons();
				break;
			case 2:
				currentMenu = itemsUI;
				break;
			}

			currentMenu.enable();

			auto& script = currentMenu.getComponent<Engine::Components::ScriptComponent>();
			if (script) {
				auto mgr = dynamic_cast<ButtonManager*>(script.instance);
				mgr->onSelect([&](int val) {
					onSelect(val);
				});
			}

		}
	}
}

void BattleUIPlayer::performAction(std::function<void(std::tuple<Player::Action, int>)> onPicked) {
	currentMenu.enable();
	isShown = true;
	this->onPicked = onPicked;
	auto& script = currentMenu.getComponent<Engine::Components::ScriptComponent>();
	if (script) {
		auto mgr = dynamic_cast<ButtonManager*>(script.instance);
		mgr->onSelect([&](int val) {
			onSelect(val);
		});
	}
}

void BattleUIPlayer::onSelect(int val) {
	Player::Action pickedAction;
	switch (currentMenuId) {
	case 0:
		pickedAction = Player::DEAL_DAMAGE;
		break;
	case 1:
		pickedAction = Player::SWITCH_WEAPON;
		break;
	case 2:
		pickedAction = Player::USE_ITEM;
		break;
	}
	currentMenu.disable();
	isShown = false;
	onPicked({ pickedAction, val });
}

void BattleUIPlayer::createWeaponButtons() {
	Instattiate([&](Engine::Scene* scene) {
		auto battleUI = scene->getEntityByName("weaponSwitchMenu");
		
		if (!battleUI.hasComponent<Engine::Components::ScriptComponent>())
			battleUI.addComponent<Engine::Components::ScriptComponent>().bind<ButtonManager>();
		if (!battleUI.hasComponent<Engine::Components::ButtonManagerData>())
			battleUI.addComponent<Engine::Components::ButtonManagerData>();
			
		auto& uiMgrData = battleUI.getComponent<Engine::Components::ButtonManagerData>();

		auto& weapons = scene->m_GlobalStorage->get<Inventory>("inventory")->weapons;

		for (int i = 0; i < weapons.size(); i++) {
			auto& w = weapons[i];
			auto button = createDialogButton(w.name, "", i, scene);
			button.getComponent<Engine::Components::TransformComponent>().Position = { 0.0f, 70.0f + 64.0f * i };
			button.getComponent<Engine::Components::BackgroundComponent>().scale = { 4.0, 4.0 };

			if (i == 0) {
				uiMgrData.currentButton = button;
			}
			button.setParent(battleUI);
		}

		return battleUI;
	});
}
