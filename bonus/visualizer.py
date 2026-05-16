import sys
import pygame
import math
import re

# --- CONFIGURATION GRAPHIQUE ---
WIDTH, HEIGHT = 900, 700
FPS = 60

# Couleurs (RGB)
COLORS = {
    "idle": (130, 130, 130),         # Gris (En attente)
    "compiling": (46, 204, 113),     # Vert (Compilation)
    "debugging": (52, 152, 219),     # Bleu (Débogage)
    "refactoring": (241, 196, 15),   # Jaune (Refactoring)
    "dead": (231, 76, 60),           # Rouge (Burnout)
    "done": (236, 240, 241)          # Blanc (Mission accomplie)
}

LEGEND_LABELS = {
    "idle": "En attente / Dongle",
    "compiling": "Compilation",
    "debugging": "Debogage",
    "refactoring": "Refactoring",
    "dead": "Burnout (Mort)",
    "done": "Termine"
}

# --- CHARGEMENT ET PARSING DES LOGS ---
def load_and_parse_logs(filename):
    logs = []
    max_coder_id = 0
    log_pattern = re.compile(r"(\d+)\s+(\d+)\s+(.+)")
    
    try:
        with open(filename, "r") as file:
            for line in file:
                line = line.strip()
                match = log_pattern.match(line)
                if match:
                    timestamp = int(match.group(1))
                    coder_id = int(match.group(2))
                    action = match.group(3)
                    
                    if coder_id > max_coder_id:
                        max_coder_id = coder_id
                        
                    logs.append({
                        "time": timestamp,
                        "id": coder_id,
                        "action": action,
                        "raw": line
                    })
    except FileNotFoundError:
        print(f"Erreur : Le fichier '{filename}' est introuvable.")
        sys.exit(1)
        
    # Trier les logs par timestamp pour être sûr de la chronologie
    logs.sort(key=lambda x: x["time"])
    return logs, max_coder_id + 1

# --- CALCUL DE L'ÉTAT À UN INSTANT T ---
def get_simulation_state_at(target_time, logs, num_coders):
    """
    Rejoue les logs depuis le début jusqu'à target_time pour reconstruire 
    l'état exact des codeurs, des statistiques et des derniers messages.
    """
    states = {i: "idle" for i in range(num_coders)}
    stats = {"compilations": 0, "active": num_coders, "burnouts": 0}
    recent_logs = []
    
    for log in logs:
        if log["time"] > target_time:
            break
            
        coder_id = log["id"]
        action = log["action"]
        recent_logs.append(log["raw"])
        
        if "is compiling" in action:
            states[coder_id] = "compiling"
        elif "is debugging" in action:
            states[coder_id] = "debugging"
        elif "is refactoring" in action:
            states[coder_id] = "refactoring"
        elif "burned out" in action:
            states[coder_id] = "dead"
            stats["burnouts"] += 1
            stats["active"] -= 1
        elif "done the job" in action:
            states[coder_id] = "done"
            stats["active"] -= 1
            
        if "compiled for the" in action:
            stats["compilations"] += 1
            
    # Garder seulement les 5 derniers logs pour l'affichage textuel
    return states, stats, recent_logs[-5:]

# --- FONCTION PRINCIPALE ---
def main():
    filename = sys.argv[1] if len(sys.argv) > 1 else "simulation_logs.txt"
    all_logs, num_coders = load_and_parse_logs(filename)
    
    if not all_logs:
        print("Le fichier de logs est vide ou mal formate.")
        return

    max_time = all_logs[-1]["time"]

    # Initialisation Pygame
    pygame.init()
    screen = pygame.display.set_mode((WIDTH, HEIGHT))
    pygame.display.set_caption("Codexion Log Player Pro")
    clock = pygame.time.Clock()
    
    # Polices
    font_title = pygame.font.SysFont("Arial", 20, bold=True)
    font_base = pygame.font.SysFont("Arial", 16)
    font_bold = pygame.font.SysFont("Arial", 16, bold=True)
    
    # Cache pour les numéros de codeurs
    id_cache = {i: font_title.render(str(i), True, (0, 0, 0)) for i in range(num_coders)}

    # Variables de contrôle du temps
    virtual_time = 0.0
    playback_speeds = [0.25, 0.5, 1.0, 2.0, 5.0, 10.0]
    speed_index = 2  # Par défaut à 1.0x
    is_paused = False

    running = True
    while running:
        delta_ms = clock.tick(FPS)
        
        # 1. Gestion des entrées clavier (Contrôles)
        for event in pygame.event.get():
            if event.type == pygame.QUIT:
                running = False
            elif event.type == pygame.KEYDOWN:
                if event.key == pygame.K_SPACE:  # Pause / Lecture
                    is_paused = not is_paused
                elif event.key == pygame.K_UP:    # Augmenter vitesse
                    if speed_index < len(playback_speeds) - 1:
                        speed_index += 1
                elif event.key == pygame.K_DOWN:  # Diminuer vitesse
                    if speed_index > 0:
                        speed_index -= 1
                elif event.key == pygame.K_RIGHT: # Avancer de 1s
                    virtual_time = min(max_time, virtual_time + 1000)
                elif event.key == pygame.K_LEFT:  # Reculer de 1s
                    virtual_time = max(0, virtual_time - 1000)

        # 2. Écoulement du temps virtuel
        if not is_paused and virtual_time < max_time:
            virtual_time += delta_ms * playback_speeds[speed_index]
            if virtual_time > max_time:
                virtual_time = max_time

        # 3. Calcul de l'état de la frame actuelle
        current_states, current_stats, text_logs = get_simulation_state_at(int(virtual_time), all_logs, num_coders)

        # 4. RENTRÉE GRAPHIQUE (Dessin)
        screen.fill((24, 24, 24))  # Fond ultra-sombre élégant

        # --- PANNEAU LÉGENDE (Gauche) ---
        leg_x, leg_y = 30, 40
        screen.blit(font_title.render("LÉGENDE", True, (255, 255, 255)), (leg_x, leg_y - 25))
        for state, color in COLORS.items():
            pygame.draw.circle(screen, color, (leg_x + 10, leg_y + 10), 10)
            lbl = font_base.render(LEGEND_LABELS[state], True, (180, 180, 180))
            screen.blit(lbl, (leg_x + 30, leg_y + 2))
            leg_y += 35

        # --- PANNEAU STATISTIQUES (Haut Droite) ---
        stat_x, stat_y = WIDTH - 280, 40
        screen.blit(font_title.render("STATISTIQUES", True, (255, 255, 255)), (stat_x, stat_y - 25))
        stats_text = [
            f"Temps de sim : {int(virtual_time)} / {max_time} ms",
            f"Vitesse : {playback_speeds[speed_index]}x" + (" (PAUSE)" if is_paused else ""),
            f"Compilations : {current_stats['compilations']}",
            f"Codeurs actifs : {current_stats['active']}",
            f"Burnouts : {current_stats['burnouts']}"
        ]
        for txt in stats_text:
            lbl = font_base.render(txt, True, (200, 200, 200))
            screen.blit(lbl, (stat_x, stat_y))
            stat_y += 25

        # --- COMMANDE COMPORTEMENT TIMELINE (Bas) ---
        # Dessin de la barre de progression
        bar_x, bar_y, bar_w, bar_h = 50, HEIGHT - 140, WIDTH - 100, 8
        pygame.draw.rect(screen, (60, 60, 60), (bar_x, bar_y, bar_w, bar_h)) # Fond de la barre
        progress_w = int((virtual_time / max_time) * bar_w) if max_time > 0 else 0
        pygame.draw.rect(screen, (52, 152, 219), (bar_x, bar_y, progress_w, bar_h)) # Remplissage bleu
        pygame.draw.circle(screen, (255, 255, 255), (bar_x + progress_w, bar_y + 4), 7) # Curseur

        # --- CONSOLE DE LOGS TEXTUELS (Tout en bas) ---
        console_y = HEIGHT - 110
        pygame.draw.rect(screen, (15, 15, 15), (50, console_y, WIDTH - 100, 90)) # Cadre console
        for idx, log_line in enumerate(text_logs):
            lbl = font_base.render(f">  {log_line}", True, (46, 204, 113) if "compiled" in log_line else (200, 200, 200))
            screen.blit(lbl, (65, console_y + 5 + (idx * 16)))

        # --- DESSIN DES CODEURS (Cercle Central) ---
        center_x, center_y = WIDTH // 2 - 20, HEIGHT // 2 - 40
        radius = 160
        for i in range(num_coders):
            angle = (i / max(1, num_coders)) * 2 * math.pi - (math.pi / 2)
            cx = center_x + int(math.cos(angle) * radius)
            cy = center_y + int(math.sin(angle) * radius)

            state = current_states.get(i, "idle")
            color = COLORS.get(state, COLORS["idle"])

            # Effet d'épaisseur/bordure si actif
            pygame.draw.circle(screen, color, (cx, cy), 35)
            pygame.draw.circle(screen, (30, 30, 30), (cx, cy), 35, 3) # Bordure interne
            
            # Affichage ID (Centré avec le cache)
            txt_rect = id_cache[i].get_rect(center=(cx, cy))
            screen.blit(id_cache[i], txt_rect)

        pygame.display.flip()

    pygame.quit()

if __name__ == "__main__":
    main()