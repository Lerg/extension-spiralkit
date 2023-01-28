#ifndef APP_PLAY_SCENE_H
#define APP_PLAY_SCENE_H

#include <spiralkit/scene_manager.h>
#include <spiralkit/ui/button.h>
#include <spiralkit/ui/ui.h>
#include "../app.h"
#include "../assets.h"
#include "../hashes.h"

using namespace spiralkit;

namespace app {
	class Piece : public ui::Button {
		public:
			uint32_t index = 0;
			Vector2i gridPosition = Vector2i(0, 0);
			Piece(SkObject *parent):
			ui::Button(assets::sprites::piece, assets::sprites::piece_pressed, assets::labels::main, parent) {

			}
	};

	class PlayScene : public Scene {
		private:
			SkSprite _background;
			ui::Button _backButton;
			SkLabel _label;
			dmArray<SkSprite> _placeholders;
			dmArray<Piece> _pieces;
			const uint32_t _count = 16;
			const Vector2i _gridSize = Vector2i(4, 4);
			Vector2 *_touchStart = nullptr;
			float _pieceSpacing = 0;
			bool _haveMoved = false;
			RandomPCG _rng;

		public:
			#pragma region Scene

			PlayScene()
			: Scene(hashes::play)
			, _background(SkSprite(assets::sprites::background, this))
			, _backButton(ui::Button(assets::sprites::buttons$button, assets::sprites::buttons$button_pressed, assets::labels::main, this))
			, _label(SkLabel(assets::labels::main, this)) {
				_backButton.GetLabel()->SetText("BACK");
				_label.SetText("This is the game.\n15 Puzzle");

				_backButton.onRelease = [this](ui::Button &button) {
					SceneManager::Show(hashes::menu, SceneTransition_SlideRight);
				};

				_placeholders.SetCapacity(_count);
				_placeholders.SetSize(_count);
				for (uint32_t i = 0; i < _count; ++i) {
					SkSprite *placeholder = &_placeholders[i];
					new(placeholder) SkSprite(assets::sprites::placeholder, this);
				}

				auto piece_touch_press_function = [this](ui::Button &button) {
					_haveMoved = false;
				};

				auto piece_touch_function = [this](ui::Button &button, const TouchEvent &touch_event) {
					if (_haveMoved) {
						return;
					}
					Piece *piece = (Piece*)&button;
					if (_touchStart == nullptr) {
						_touchStart = new Vector2(touch_event.position);
					} else {
						Vector2 dir = touch_event.position - *_touchStart;
						if (dir.length() > 30) {
							int32_t d = (int32_t)Math::round((dir.angle()) / (0.5 * Math_PI));
							switch (d) {
								case 0:
									_MovePiece(piece, Vector2i(1, 0));
									break;
								case 1:
									_MovePiece(piece, Vector2i(0, 1));
									break;
								case -1:
									_MovePiece(piece, Vector2i(0, -1));
									break;
								case -2:
								case 2:
									_MovePiece(piece, Vector2i(-1, 0));
									break;
							}
							_haveMoved = true;
							delete _touchStart;
							_touchStart = nullptr;
						}
					}

				};

				_rng.randomize();

				dmArray<uint32_t> numbers;
				numbers.SetCapacity(_count - 1);
				for (uint32_t i = 0; i < numbers.Capacity(); ++i) {
					numbers.Push(i);
				}

				_pieces.SetCapacity(_count - 1);
				_pieces.SetSize(_pieces.Capacity());
				for (uint32_t i = 0; i < _pieces.Size(); ++i) {
					Piece *piece = &_pieces[i];
					new(piece) Piece(this);
					piece->index = i;
					piece->gridPosition = Vector2i(i % _gridSize.width, i / _gridSize.height);

					uint32_t number_index = _rng.random(0, numbers.Size() - 1);
					uint32_t number = numbers[number_index];
					dmArrayUtil::Remove(&numbers, number_index);

					piece->GetLabel()->SetTextF("%d", number + 1);
					piece->onMove = piece_touch_function;
					piece->onPress = piece_touch_press_function;
				}
			}

			bool _IsFreePosition(Vector2i grid_position) {
				for (uint32_t i = 0; i < _pieces.Size(); ++i) {
					Piece *piece = &_pieces[i];
					if (piece->gridPosition == grid_position) {
						return false;
					}
				}
				return true;
			}

			void _MovePiece(Piece *piece, Vector2i dir) {
				Vector2i grid_position = piece->gridPosition + dir;
				if (grid_position.x < 0) {
					grid_position.x = 0;
				}
				if (grid_position.x >= _gridSize.width) {
					grid_position.x = _gridSize.width - 1;
				}
				if (grid_position.y < 0) {
					grid_position.y = 0;
				}
				if (grid_position.y >= _gridSize.height) {
					grid_position.y = _gridSize.height - 1;
				}
				if (_IsFreePosition(grid_position)) {
					piece->gridPosition = grid_position;
					piece->AnimatePosition(_pieceSpacing * (Vector2(piece->gridPosition) - (0.5 * Vector2(_gridSize) - Vector2(0.5, 0.5))), 0.25);
				}
			}

			void OnWillShow(void *userdata) {
			}

			void OnDidHide() {
				if (_touchStart != nullptr) {
					delete _touchStart;
				}
			}

			void OnUpdate(float dt) {
			}

			void OnResize(Vector2i screen_size) {
				ui::LetterboxFit(&_background, screen_size);

				float scale = 25 * (float)assets::sprites::buttons$button.width / screen_size.width;
				_backButton.SetPosition(Vector2(-0.35 * screen_size.width, 0.35 * screen_size.height));
				_backButton.SetScale(scale);

				_label.SetScale(0.2 * scale);
				_label.SetPosition(Vector2(0, 0.4 * screen_size.height));

				float spacing = 0.1 * screen_size.width;
				float placeholder_scale = spacing / assets::sprites::placeholder.width;
				for (int32_t y = 0; y < _gridSize.height; ++y) {
					for (int32_t x = 0; x < _gridSize.width; ++x) {
						SkSprite *placeholder = &_placeholders[x + y * _gridSize.width];
						placeholder->SetPosition(spacing * Vector2(x - _gridSize.x / 2 + 0.5, y - _gridSize.y / 2 + 0.5));
						placeholder->SetScale(placeholder_scale);
					}
				}

				_pieceSpacing = spacing;
				for (uint32_t i = 0; i < _pieces.Size(); ++i) {
					Piece *piece = &_pieces[i];
					piece->SetPosition(spacing * (Vector2(piece->gridPosition) - (0.5 * Vector2(_gridSize) - Vector2(0.5, 0.5))));
					piece->SetScale(0.9 * placeholder_scale);
				}
			}

			#pragma endregion

		private:
	};
}

#endif
