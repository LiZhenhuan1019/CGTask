#include <cmath>
#include <glm/ext/scalar_constants.hpp>
#include <glm/glm.hpp>

namespace CGTask::math
{
    struct point
    {
        double x, y;
    };
    class elliptic_orbit
    {
    public:
        // e^2 = c^2 / a^2 = 1 - b^2 / a^2
        // b = sqrt((1 - e^2) * a^2)
        elliptic_orbit(double e, double semi_major_axis)
            : e(e), a(semi_major_axis),
              b(std::sqrt((1 - e * e) * a * a))
        {}
        // y^2 = b^2 * (1 - x^2 / a^2);
        point position_on_elliptic_orbit(double t, double period) const
        {
            double M = 2 * glm::pi<double>() * t / period;
            double E = solve_equation(M);
            double x_divide_a = std::cos(E);
            double y = b * std::sqrt(1 - x_divide_a * x_divide_a);
            if (t > period / 2)
                return {x_divide_a * a, -y};
            else
                return {x_divide_a * a, y};
        }
    private:
        double e;
        double a;
        double b;

        // M = E - e * sin(E)
        double solve_equation(double M) const
        {
            return solve_equation(0.0001, M, 0, 7);
        }
        double solve_equation(double precision, double M, double E_left, double E_right) const
        {
            double M_left = equation_value(E_left);
            if (M == M_left)
                return E_left;
            double M_right = equation_value(E_right);
            if (M == M_right)
                return E_right;
            while (E_right - E_left > precision)
            {
                double E_middle = (E_left + E_right) / 2;
                double M_middle = equation_value(E_middle);
                if (M > M_middle)
                {
                    E_left = E_middle;
                    M_left = M_middle;
                } else if (M < M_middle)
                {
                    E_right = E_middle;
                    M_right = M_middle;
                }
                else
                    return M_middle;
            }
            return (E_right + E_left) / 2;
        }
        double equation_value(double E) const
        {
            return E - e * std::sin(E);
        }

    };
}
