#pragma once
#include <glm/geometric.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <bitset>
#include <algorithm>
#include <glm/trigonometric.hpp>
#include <math.h>

namespace CGTask::camera
{
    struct input_enum
    {
        enum 
        {
            forward, backward, left, right, up, down
        };
    };
    
    class free_camera
    {
    public:
        free_camera(glm::vec3 pos, glm::vec3 dir, std::size_t width, std::size_t height)
            : position(pos), forward(glm::normalize(dir)), width(width), height(height)
        {
            set_dir(dir);
            update_right_and_up();
            update_view();
            update_projection();
        }
        glm::vec3 pos() const
        {
            return position;
        }
        glm::mat4 view() const
        {
            return view_matrix;
        }
        glm::mat4 projection() const
        {
            return projection_matrix;
        }
        void go_forward(float speed)
        {
            position += speed * forward;
            update_view();
        }
        void go_backward(float speed)
        {
            position -= speed * forward;
            update_view();
        }
        void go_left(float speed)
        {
            position -= speed * right;
            update_view();
        }
        void go_right(float speed)
        {
            position += speed * right;
            update_view();
        }
        void go_up(float speed)
        {
            position += speed * up;
            update_view();
        }
        void go_down(float speed)
        {
            position -= speed * up;
            update_view();
        }
        void move(std::bitset<6> const &input, float speed)
        {
            move_impl(input, speed);
            update_view();
        }
        void turn(double x_offset, double y_offset)
        {
            turn_impl(x_offset, y_offset);
            update_right_and_up();
            update_view();
        }

        void orthographic_scale(double scale)
        {
            orthographic_scale_ = scale;
            update_projection();
        }
        void set_size(std::size_t new_width, std::size_t new_height)
        {
            width = new_width;
            height = new_height;
            update_projection();
        }
        void orthographic()
        {
            is_orthographic_ = true;
            update_projection();
        }
        void perspective()
        {
            is_orthographic_ = false;
            update_projection();
        }
        void toggle_projection()
        {
            is_orthographic_ = !is_orthographic_;
            update_projection();
        }
        bool is_orthographic() const
        {
            return is_orthographic_;
        }
    private:
        void set_dir(glm::vec3 dir)
        {
            dir = glm::normalize(dir);
            if (dir.x == 0 && dir.y == 0)
            {
                pitch = glm::degrees(asin(dir.z));
                yaw = 0;
                return;
            }
            double pitch_rad = asin(dir.z);
            double cos_pitch_rad = cos(pitch_rad);
            if (cos_pitch_rad == 0)
                cos_pitch_rad += 0.0000000000001;
            yaw = glm::degrees(-asin(dir.y / cos_pitch_rad));
            if (dir.x < 0)
                yaw = 180 - yaw;
            pitch = glm::degrees(pitch_rad);
        }
        void update_right_and_up()
        {
            right = glm::normalize(glm::cross(forward, glm::vec3(0, 0, 1.0f)));
            up = glm::cross(right, forward);
        }
        void update_view()
        {
            view_matrix = glm::lookAt(position, position + forward, up);
        }

        void update_projection()
        {
            if (is_orthographic_)
                projection_matrix = glm::ortho(
                        -(float)width / (2 * orthographic_scale_),
                        (float)width / (2 * orthographic_scale_),
                        -(float)height / (2 * orthographic_scale_),
                        (float)height / (2 * orthographic_scale_),
                        0.01f, 1000.f);
            else
                projection_matrix = glm::perspective(glm::radians(45.0f), (float)width/(float)height, 0.01f, 1000.0f);
        }
        void move_impl(std::bitset<6> const &input, float speed)
        {
            int forward_backward = input.test(input_enum::forward) - input.test(input_enum::backward);
            int right_left = input.test(input_enum::right) - input.test(input_enum::left);
            int up_down = input.test(input_enum::up) - input.test(input_enum::down);
            position += speed * forward_backward * forward;
            position += speed * right_left * right;
            position += speed * up_down * up;
        }
        void turn_impl(double x_offset, double y_offset)
        {
            yaw += x_offset;
            if (yaw > 360)
                yaw -= 360;
            else if (yaw < -360)
                yaw += 360;
            pitch += y_offset;
            pitch = std::clamp(pitch, -89.0f, 89.0f);

            glm::vec3 front;
            front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
            front.y = cos(glm::radians(pitch)) * (-sin(glm::radians(yaw)));
            front.z = sin(glm::radians(pitch));
            forward = glm::normalize(front);
        }

        glm::vec3 position;
        glm::vec3 forward;
        std::size_t width, height;
        float orthographic_scale_ = 1;
        bool is_orthographic_ = false;
        float pitch = 0, yaw = 0;


        glm::vec3 right;
        glm::vec3 up;
        glm::mat4 view_matrix;
        glm::mat4 projection_matrix;
    };
}
