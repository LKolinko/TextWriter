#pragma once
#include "Button.h"
#include "Screen.h"
#include "Singleton.h"
#include "TextLabel.h"
#include "EnterWindow.h"
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Font.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Vector2.hpp>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <SFML/Graphics.hpp>
class ButtonLable : public Singleton<ButtonLable>{
public:
	ButtonLable() = default;
    ~ButtonLable() {
        for (auto& btn : buttons_) {
            delete btn;
        }
    }
	void create(sf::Vector2u sizeWindow, int cnt, sf::Font& font) {
		buttons_.resize(cnt);
		names.resize(cnt);
        names = { "Insert", "Erase", "insert substr", "replace char", "finde and replace", "erase leading zeros", "* -> +", "erase brackets", "erase '/'", "Terminal" };
		float size_x = (float)sizeWindow.x * (float)0.25;
		float px = (float)sizeWindow.x - size_x * (float)1.05;
		float py = (float)sizeWindow.y * (float)0.01;
		float dy = (float)sizeWindow.y / (float)cnt;
		float size_y = (float)dy * (float)0.7;
	       	for (int i = 0; i < cnt; ++i, py += dy) {
			auto btn = new Button();
			btn->setSize({ size_x, size_y });
            btn->setText(names[i], std::min((int)size_x, (int)size_y) / 2, sf::Color({ 38, 15, 38 }), font);
			btn->setPosition({ px, py });
			btn->setBackColor(sf::Color({ 201, 201, 238 }));
			buttons_[i] = btn;
		}
	}
	void toDrawOn() {
		for (auto& btn : buttons_) {
			btn->drawTo();
		}
	}
	void OnClick(sf::Font& font) {
		for (int i = 0; i < buttons_.size(); ++i) {
			if (buttons_[i]->isMouseOver()) {
				buttons_[i]->setBackColor(sf::Color({ 101, 100, 219 }));
				if (i == buttons_.size() - 1 && !EnterWindow::GetInstance()->flag) {
					EnterWindow::GetInstance()->open();
				} else {
                    std::string input = EnterWindow::GetInstance()->getString();
                    EnterWindow::GetInstance()->Clear();
                    std::stringstream ss(input);
                    if (EnterWindow::GetInstance()->flag) {
                        EnterWindow::GetInstance()->Close();
                    }
                    if (i == 0) {
                        std::string res_str;
                        int n = 0;
                        ss >> n;
                        if (!input.empty()) {
                            std::reverse(input.begin(), input.end());
                            while (input.back() != '|') {
                                input.pop_back();
                            }
                            input.pop_back();
                            std::reverse(input.begin(), input.end());
                            for (int j = 0; j < input.size(); ++j) {
                                res_str.push_back(input[j]);
                                if (input[j] == '\n' || j == input.size() - 1) {
                                    if (!res_str.empty()) {
                                        if (res_str.back() == '\n') {
                                            res_str.pop_back();
                                        }
                                        InsertAfterN(res_str, n++, font);
                                    }
                                    ++j;
                                    res_str.clear();
                                }
                            }
                        }
                        TransferUp(font);
                    } else if (i == 1) {
                        int n = -1;
                        ss >> n;
                        if (n == -1 || n >= text_.size()) {
                            std::cout << "ERROR" << std::endl;
                        } else {
                            if (n != 1) {
                                bool flag = false;
                                for (int j = 0; j < text_[n - 1].size(); ++j) {
                                    if (text_[n - 1][j] == '~') {
                                        flag = true;
                                        break;
                                    }
                                }
                                if (flag && text_[n - 2].back() != '~') {
                                    text_[n - 2] += " ~";
                                }
                            }
                            text_.erase(text_.begin() + n - 1);
                        }
                        TransferUp(font);
                    } else if (i == 2) {
                        int n = -1, m = -1;
                        ss >> n >> m;
                        if (n == -1 || m == -1 || n > text_.size() || text_[n - 1][m - 1] == '~') {
                            std::cout << "ERROR" << std::endl;
                        } else {
                            std::string str, buffer;
                            while (ss >> buffer) {
                                if (!str.empty()) {
                                    str += ' ';
                                }
                                str += buffer;
                            }
                            for (int j = 0; j < str.size(); ++j) {
                                text_[n - 1].insert(text_[n - 1].begin() + m + j - 1, str[j]);
                            }
                            TransferUp(font);
                        }
                    } else if (i == 3) {
                        int n = -1, m = -1;
                        ss >> n >> m;
                        if (n == -1 || m == -1 || n > text_.size() || text_[n - 1][m - 1] == '~') {
                            std::cout << "ERROR" << std::endl;
                        } else {
                            std::string c;
                            ss >> c;
                            text_[n - 1][m - 1] = c[0];
                            TransferUp(font);
                        }
                    } else if (i == 4) {
                        int n = 1, m = text_.size(), j = 0;
                        if (input[0] >= '0' && input[0] <= '9') {
                            ss >> n >> m;
                            while (input[j] != '|' && j < input.size()) {
                                ++j;
                            }
                            ++j;
                        }
                        if (input[0] == '|') {
                            ++j;
                        }
                        std::string find, replase;
                        while (input[j] != '|' && j < input.size()) {
                            find.push_back(input[j]);
                            ++j;
                        }
                        ++j;
                        while (j < input.size()) {
                            replase.push_back(input[j]);
                            ++j;
                        }
                        int l = 0, start = 0;
                        if (!find.empty() && !replase.empty()) {
                            for (j = n - 1; j < m; ++j) {
                                for (int k = 0; k < text_[j].size(); ++k) {
                                    if (l == find.size()) {
                                        for (int u = 0, ind = 0; u < std::max(replase.size(), find.size()); ++u, ++ind) {
                                            if (u >= find.size() && u < replase.size()) {
                                                text_[j].insert(text_[j].begin() + u + start, replase[u]);
                                            } else if (u >= replase.size()) {
                                                text_[j].erase(text_[j].begin() + ind + start);
                                                --ind;
                                            } else {
                                                text_[j][u + start] = replase[u];
                                            }
                                        }
                                        l = 0;
                                    }
                                    if (text_[j][k] == find[l]) {
                                        if (l == 0) {
                                            start = k;
                                        }
                                        l++;
                                    } else {
					if (start != 0) {
                                            k = start + 1;
					}
                                        l = 0;
                                        start = 0;
                                    }
                                }
                            }
                            TransferUp(font);
                        }
                    } else if (i == 5) {
                        int n = 1, m = (int)text_.size();
                        if (*input.begin() != '|') {
                            ss >> n >> m;
                        }
                        for (int j = n - 1; j < m; ++j) {
                            bool flag = true;
                            for (int k = 0; k < text_[j].size() - 1; ++k) {
                                if (text_[j][k] == '0' && flag && text_[j][k + 1] >= '0' && text_[j][k + 1] <= '9') {
                                    text_[j].erase(text_[j].begin() + k);
                                    --k;
                                } else if (text_[j][k] > '0' && text_[j][k] <= '9') {
                                    flag = false;
                                } else if (!(text_[j][k] >= '0' && text_[j][k] <= '9')) {
                                    flag = true;
                                }
                            }
                        }
                        TransferUp(font);
                    } else if (i == 6) {
                        int n = 1, m = (int)text_.size();
                        char ch;
                        if (*input.begin() >= '0' && *input.begin() <= '9') {
                            ss >> n >> m;
                        }
                        ss >> ch;
                        for (int j = n - 1; j < m; ++j) {
                            int start = -1, finish = -1;
                            for (int k = 0; k < text_[j].size(); ++k) {
                                if (text_[j][k] == ch && start == -1) {
                                    start = k;
                                }
                                if (text_[j][k] == ch) {
                                    finish = k;
                                }
                                if (text_[j][k] != ch || k == text_[j].size() - 1) {
                                    if (finish - start > 0) {
                                        int l = start;
                                        for (int cnt = 0; cnt <= (finish - start) / 2; ++l, ++cnt) {
                                            text_[j][l] = '+';
                                        }
                                        int past = l;
                                        for (; l <= finish; ++l) {
                                            text_[j].erase(text_[j].begin() + past);
                                        }
                                        start = -1;
                                        finish = -1;
                                    }
                                }
                            }
                        }
                        TransferUp(font);
                    } else if (i == 7) {
                        int n = 1, m = text_.size();
                        if (input[0] >= '0' && input[0] <= '9') {
                            ss >> n >> m;
                        }
                        for (int j = n - 1; j < m; ++j) {
                            bool flag = false;
                            for (int k = 0; k < text_[j].size(); ++k) {
                                if (text_[j][k] == '<' && !flag) {
                                    flag = true;
                                } else if (flag && text_[j][k] != '>') {
                                    text_[j].erase(text_[j].begin() + k);
                                    --k;
                                } else if (text_[j][k] == '>') {
                                    flag = false;
                                }
                            }
                        }
                    } else if (i == 8) {
                        int n = 1, m = text_.size();
                        if (input[0] >= '0' && input[0] <= '9') {
                            ss >> n >> m;
                        }
                        for (int j = n - 1; j < m; ++j) {
                            int start = -1, finish = -1, max;
                            bool flag = false;
                            for (int k = 0; k < text_[j].size(); ++k) {
                                if (text_[j][k] >= '0' && text_[j][k] <= '9' && start == -1) {
                                    start = k;
                                    max = text_[j][k] - '0';
                                }
                                if (text_[j][k] >= '0' && text_[j][k] <= '9') {
                                    finish = k;
                                    if (text_[j][k] - '0' < max) {
                                        flag = true;
                                    }
                                    max = std::max(max, text_[j][k] - '0');
                                }
                                if (!(text_[j][k] >= '0' && text_[j][k] <= '9')) {
                                    if (flag) {
                                        for (int l = start; l <= finish; ++l) {
                                            text_[j].erase(text_[j].begin() + start);
                                        }
                                    } else {
                                        start = -1;
                                        finish = -1;
                                    }
                                }
                            }
                        }
                    }
                    TransferUp(font);
                }
			} else {
				buttons_[i]->setBackColor(sf::Color({ 201, 201, 238 }));
			}
		}
	}
	void noClick() {
		for (int i = 0; i < buttons_.size(); ++i) {
			buttons_[i]->setBackColor(sf::Color({ 201, 201, 238 }));
		}
	}
	void insert(int n, std::string& str) {
		text_.emplace_back("");
		for (int i = text_.size() - 2; i >= n; --i) {
			text_[i + 1] = std::move(text_[i]);
		}
		text_[n] = str;
	}
	void InsertAfterN(std::string& str, int n, sf::Font& font) {
        if (n > text_.size()) {
            std::cout << "ERROR" << std::endl;
        } else {
            if (!str.empty()) {
                str += " ~";
                insert(n, str);
            }
        }
	}
	void TransferUp(sf::Font& font) {
		for (int i = 0; i < text_.size(); ++i) {
			sf::Text text;
			text.setFont(font);
			text.setCharacterSize(25);
			std::string secondString;
			text.setString(text_[i]);
			while (text.getLocalBounds().width > TextLabel::GetInstance()->getSizeText(i)) {
				secondString += text_[i][text_[i].size() - 1];
				text_[i].pop_back();
				text.setString(text_[i]);
			}	
			if (!secondString.empty()) {
				std::reverse(secondString.begin(), secondString.end());
				if (text_.size() <= i + 1) {
					insert(i + 1, secondString);
				} else {
                    text_[i + 1] = secondString + text_[i + 1];
				}
			}
		}
        TextLabel::GetInstance()->setStrings(text_);
	}
    void TransferDown(sf::Font& font) {
        for (int i = 0; i < std::max((int)text_.size() - 1, 0); ++i) {
            sf::Text text;
            text.setFont(font);
            text.setCharacterSize(25);
            text.setString(text_[i]);
            while (text.getLocalBounds().width < TextLabel::GetInstance()->getSizeText(i)) {
                if (i >= text_.size() - 1 || text_[i].back() == '~') {
                    break;
                }
                text_[i] += text_[i + 1][0];
                text_[i + 1].erase(text_[i + 1].begin());
                if (text_[i + 1].empty()) {
                    text_.erase(text_.begin() + i + 1);
                }
                if (!text_[i].empty()) {
                    text.setString(text_[i]);
                }
            }
        }
        TextLabel::GetInstance()->setStrings(text_);
    }
private:
	EnterWindow wnd;
	std::vector<Button*> buttons_;
	std::vector<std::string> names;
	std::vector<std::string> text_;
};
