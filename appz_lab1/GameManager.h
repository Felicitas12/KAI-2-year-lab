#pragma once
#include <algorithm>
#include <vector>
#include <string>
#include <memory>
#include "Platform.h"

class GameManager {
	std::vector<std::unique_ptr<Game>> library_;
	Game* active_ = nullptr;

	Game* find(const std::string& name) const {
		auto it = std::find_if(library_.begin(), library_.end(), [&](const auto& g) { return g->getName() == name; });
		return it != library_.end() ? it->get() : nullptr;
	}
	bool hasGenre(GameGenre genre) const {
		return std::any_of(library_.begin(), library_.end(), [genre](const auto& g) { return g->getGenre() == genre; });
	}

public:
	std::string addGame(std::unique_ptr<Game> game) {
		if (hasGenre(game->getGenre())) return "Гру жанру '" + genreName(game->getGenre()) + "' вже додано. Ліміт: 1 гра на жанр.";
		library_.push_back(std::move(game)); return "";
	}

	std::string installGame(const std::string& name, Platform& p) {
		auto* g = find(name);
		if (!g) return "Гру не знайдено.";
		if (g->getGenre() == GameGenre::Browser) return "Браузерні ігри не потребують інсталяції.";
		if (g->isInstalled()) return "Гра вже встановлена.";
		if (!g->getRequirements().hasSpace(p.getHardware())) return "Недостатньо місця на HDD.";
		g->install(p); return "";
	}

	std::string startGame(const std::string& name, Platform& p, UserAccount& acc) {
		if (active_ && active_->isRunning()) return "Вже запущена гра '" + active_->getName() + "'. Спочатку зупиніть її.";
		auto* g = find(name);
		if (!g) return "Гру не знайдено.";
		if (!g->isInstalled()) return "Гра не встановлена.";
		if (!acc.isLoggedIn()) return "Необхідно увійти в обліковий запис.";
		if (!g->getRequirements().metBy(p.getHardware())) return "Апаратне забезпечення не відповідає мінімальним вимогам.";

		if (p.getType() == PlatformType::Console) {
			auto* c = dynamic_cast<ConsolePlatform*>(&p);
			if (c && !c->hasControllers()) return "До консолі не підключені маніпулятори.";
		}
		if (auto* nr = dynamic_cast<INetworkRequired*>(g)) {
			if (!nr->isNetworkAvailable()) return "Відсутнє підключення до мережі для онлайн-гри.";
		}

		if (!g->start(p, acc)) return "Внутрішня помилка запуску.";
		active_ = g; return "";
	}

	std::string stopGame() {
		if (!active_ || !active_->isRunning()) return "Жодна гра не запущена.";
		active_->stop(); active_ = nullptr; return "";
	}

	std::string saveGame(const std::string& name, const std::string& slot) {
		auto* g = find(name);
		if (!g) return "Гру не знайдено.";
		if (!g->isRunning()) return "Не можна зберегти: гра не запущена.";
		g->saveState(slot); return "";
	}

	std::string loadGame(const std::string& name, const std::string& slot) {
		auto* g = find(name);
		if (!g) return "Гру не знайдено.";
		if (!g->isRunning()) return "Не можна завантажити: гра не запущена.";
		if (!g->hasSaves()) return "Немає збережених станів (нічого завантажувати).";
		if (!g->loadState(slot)) return "Збереження не знайдено.";
		return "";
	}

	std::string rateGame(const std::string& name, int stars) {
		auto* g = find(name);
		if (!g) return "Гру не знайдено.";
		g->addRating(stars); return "";
	}
	float getRating(const std::string& name) {
		auto* g = find(name); return g ? g->getRating() : 0.f;
	}
};
