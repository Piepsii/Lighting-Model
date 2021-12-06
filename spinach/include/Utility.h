#pragma once

#include <Render.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Utility {
	int64 GetCurrentTick();
	bool CreateShaderProgramFromFiles(ShaderProgram& program,
		const char* vertex_filename,
		const char* fragment_filename);
	bool CreateTextureFromFile(Texture& texture,
		const char* filename);

	struct Time {
		static Time Now()
		{
			return Time{ GetCurrentTick() };
		}

		static Time Deltatime()
		{
			// note: call only one per frame!
			static Time p = Time::Now();
			Time n = Time::Now();
			Time d = n - p;
			p = n;
			return d;
		}


		constexpr Time() = default;
		constexpr Time(const int64 duration)
			: duration(duration)
		{
		}

		Time& operator+=(const Time& rhs)
		{
			duration += rhs.duration;
			return *this;
		}

		Time& operator-=(const Time& rhs)
		{
			duration -= rhs.duration;
			return *this;
		}

		Time operator+(const Time& rhs) const
		{
			return { duration + rhs.duration };
		}

		Time operator-(const Time& rhs) const
		{
			return { duration - rhs.duration };
		}

		float AsSeconds() const
		{
			return float(duration) / 1000.0f;
		}

		float AsMilliseconds() const
		{
			return float(duration);
		}

		int64 duration{};
	};

	struct Mouse {
		Mouse() = default;

		void Update()
		{
			for (auto& button : m_buttons) {
				button.m_previous = button.m_current;
			}
		}

		int x() const
		{
			return m_x;
		}

		int y() const
		{
			return m_y;
		}

		bool ButtonDown(const int index) const
		{
			return m_buttons[index].m_current;
		}

		bool ButtonPressed(const int index) const
		{
			return m_buttons[index].m_current && !m_buttons[index].m_previous;
		}

		bool ButtonReleased(const int index) const
		{
			return !m_buttons[index].m_current && m_buttons[index].m_previous;
		}

		void OnMove(const int x, const int y)
		{
			m_x = x;
			m_y = y;
		}

		void OnButton(const int index, bool state)
		{
			m_buttons[index].m_current = state;
		}

		int m_x{};
		int m_y{};
		struct {
			bool m_current;
			bool m_previous;
		} m_buttons[8]{};
	};

	struct Keyboard {
		Keyboard() = default;

		void Update()
		{
			for (auto& key : keys) {
				key.previous = key.current;
			}
		}

		bool KeyDown(const int index) const
		{
			return keys[index].current;
		}

		bool KeyPressed(const int index) const
		{
			return keys[index].current && !keys[index].previous;
		}

		bool KeyReleased(const int index) const
		{
			return !keys[index].current && keys[index].previous;
		}

		void OnKey(const int index, bool state)
		{
			keys[index].current = state;
		}

		struct {
			bool current;
			bool previous;
		} keys[512]{};
	};

	struct Camera {
		Camera(const glm::mat4& projection = glm::mat4(1.0f))
			: m_right(0.0f)
			, m_up(0.0f)
			, m_forward(0.0f)
			, m_position(0.0f, 0.0f, 0.0f)
			, m_view(1.0f)
			, m_projection(projection)
		{
		}

		void update()
		{
			glm::vec3 ax = { 1.0f, 0.0f, 0.0f };
			glm::vec3 ay = { 0.0f, 1.0f, 0.0f };
			glm::vec3 az = { 0.0f, 0.0f, 1.0f };

			glm::mat4 ry = glm::rotate(glm::mat4(1.0f), -yaw, ay);
			ax = glm::normalize(glm::mat3(ry) * ax);
			az = glm::normalize(glm::mat3(ry) * az);

			glm::mat4 rx = glm::rotate(glm::mat4(1.0f), -pitch, ax);
			ay = glm::normalize(glm::mat3(rx) * ay);
			az = glm::normalize(glm::mat3(rx) * az);

			m_view[0][0] = ax.x; m_view[0][1] = ay.x; m_view[0][2] = az.x;
			m_view[1][0] = ax.y; m_view[1][1] = ay.y; m_view[1][2] = az.y;
			m_view[2][0] = ax.z; m_view[2][1] = ay.z; m_view[2][2] = az.z;
			m_view[3][0] = -glm::dot(m_position, ax);
			m_view[3][1] = -glm::dot(m_position, ay);
			m_view[3][2] = -glm::dot(m_position, az);

			//glm::mat4 rotation = glm::eulerAngleXYZ(m_pitch, m_yaw, m_roll);
			//glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);
			//m_view = rotation * translation;
			//
			//ax = glm::vec3(m_view[0][0], m_view[1][0], m_view[2][0]);
			//ay = glm::vec3(m_view[0][1], m_view[1][1], m_view[2][1]);
			//az = glm::vec3(m_view[0][2], m_view[1][2], m_view[2][2]);

			//glm::quat rx = glm::angleAxis(m_pitch, ax);
			//glm::quat ry = glm::angleAxis(m_yaw, ay);
			//glm::quat rz = glm::angleAxis(m_roll, az);
			//
			//glm::quat orientation = glm::normalize(rx * ry * rz);
			//glm::mat4 rotation = glm::mat4_cast(orientation);
			//glm::mat4 translation = glm::translate(glm::mat4(1.0f), -m_position);
			//m_view = rotation * translation;
			//
			//ax = glm::normalize(glm::conjugate(orientation) * ax);
			//ay = glm::normalize(glm::conjugate(orientation) * ay);
			//az = glm::normalize(glm::conjugate(orientation) * az);

			m_right = ax;
			m_up = ay;
			m_forward = az;
		}

		void move_x(const float amount)
		{
			m_position += m_right * amount;
		}

		void move_y(const float amount)
		{
			m_position += m_up * amount;
		}

		void move_z(const float amount)
		{
			m_position += m_forward * amount;
		}

		void rotate_x(const float amount)
		{
			pitch += amount;
		}

		void rotate_y(const float amount)
		{
			yaw += amount;
		}

		void rotate_z(const float amount)
		{
			roll += amount;
		}

		float pitch{};
		float yaw{};
		float roll{};
		glm::vec3 m_right;
		glm::vec3 m_up;
		glm::vec3 m_forward;
		glm::vec3 m_position;
		glm::mat4 m_view;
		glm::mat4 m_projection;
	};

	struct Controller {
		Controller(Camera& camera)
			: m_camera(camera)
			, m_speed(10.0f)
			, m_yaw(0.022f)
			, m_pitch(0.022f)
			, m_sensitivity(0.05f)
			, m_mouse_position(0.0f)
		{
		}

		void update(const Keyboard& keyboard, const Mouse& mouse, const Time& deltatime)
		{
			if (keyboard.KeyDown(GLFW_KEY_W)) {
				m_camera.move_z(-m_speed * deltatime.AsSeconds());
			}
			if (keyboard.KeyDown(GLFW_KEY_S)) {
				m_camera.move_z(m_speed * deltatime.AsSeconds());
			}
			if (keyboard.KeyDown(GLFW_KEY_A)) {
				m_camera.move_x(-m_speed * deltatime.AsSeconds());
			}
			if (keyboard.KeyDown(GLFW_KEY_D)) {
				m_camera.move_x(m_speed * deltatime.AsSeconds());
			}
			if (keyboard.KeyDown(GLFW_KEY_Q)) {
				m_camera.move_y(-m_speed * deltatime.AsSeconds());
			}
			if (keyboard.KeyDown(GLFW_KEY_E)) {
				m_camera.move_y(m_speed * deltatime.AsSeconds());
			}

			const glm::vec2 mouse_position(float(mouse.x()), float(mouse.y()));
			const glm::vec2 mouse_delta = mouse_position - m_mouse_position;
			if (mouse.ButtonDown(GLFW_MOUSE_BUTTON_LEFT)) {
				m_camera.rotate_x(mouse_delta.y * m_sensitivity * m_pitch);
				m_camera.rotate_y(mouse_delta.x * m_sensitivity * m_yaw);
			}

			m_mouse_position = mouse_position;
			m_camera.update();
		}

		void set_camera_speed(const float speed)
		{
			m_speed = speed;
		}

		void set_mouse_invert_yaw(const bool invert)
		{
			m_yaw = invert ? -m_yaw : m_yaw;
		}

		void set_mouse_invert_pitch(const bool invert)
		{
			m_pitch = invert ? -m_pitch : m_pitch;
		}

		void set_mouse_sensitivity(const float sensitivity)
		{
			m_sensitivity = sensitivity;
		}

		Camera& m_camera;
		float m_speed;
		float m_yaw;
		float m_pitch;
		float m_sensitivity;
		glm::vec2 m_mouse_position;
	};
}

