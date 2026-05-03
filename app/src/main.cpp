#include <zephyr/drivers/gpio.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>

/* The devicetree node identifier for the "app-led" alias. */
#define LED_NODE DT_ALIAS(app_led)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

static int get_blink_sleep_ms(void)
{
    return CONFIG_APP_HEARTBEAT_PERIOD_MS;
}

int main(void)
{
    bool led_state = true;

#ifdef CONFIG_LED_BRIGHTNESS
    const int brightness = CONFIG_LED_BRIGHTNESS;
#else
    const int brightness = 100;
#endif

#ifdef CONFIG_LED_FADE_DURATION_MS
    const int fade_duration_ms = CONFIG_LED_FADE_DURATION_MS;
#else
    const int fade_duration_ms = 0;
#endif

#ifdef CONFIG_LED_DEBUGGING
    const bool led_debug_enabled = true;
#else
    const bool led_debug_enabled = false;
#endif

#ifdef CONFIG_LED_CUSTOM_BLINK_PATTERN
    const bool custom_pattern_enabled = true;
#else
    const bool custom_pattern_enabled = false;
#endif

    const int blink_sleep_ms = get_blink_sleep_ms();

    LOG_INF("LED config: blink=%dms, brightness=%d, fade=%dms, debug=%d, custom_pattern=%d",
            blink_sleep_ms, brightness, fade_duration_ms,
            led_debug_enabled ? 1 : 0, custom_pattern_enabled ? 1 : 0);

    if (!gpio_is_ready_dt(&led)) {
        LOG_ERR("LED GPIO is not ready");
        return 0;
    }

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) {
        LOG_ERR("Failed to configure LED GPIO");
        return 0;
    }

    while (1) {
        if (gpio_pin_toggle_dt(&led) < 0) {
            LOG_ERR("Failed to toggle LED GPIO");
            return 0;
        }

        led_state = !led_state;
        LOG_INF("LED state: %s", led_state ? "ON" : "OFF");

        if (led_debug_enabled) {
            LOG_INF("debug: sleep=%dms brightness=%d fade=%dms", blink_sleep_ms, brightness,
                    fade_duration_ms);
        }

        if (custom_pattern_enabled) {
            k_msleep(blink_sleep_ms / 2);
            continue;
        }

        k_msleep(blink_sleep_ms);
    }

    return 0;
}
