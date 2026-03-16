<style>
  .futuristic-body {
    background: linear-gradient(135deg, #000000 0%, #0a0a0a 50%, #1a0033 100%);
    color: #ffffff;
    font-family: 'Courier New', monospace;
    margin: 0;
    padding: 20px;
  }
  .hero-container {
    text-align: center;
    padding: 60px 20px;
    position: relative;
  }
  .hero-title {
    font-size: 4em;
    background: linear-gradient(45deg, #00ffff, #ff00ff, #00ff00, #ffff00);
    background-size: 400% 400%;
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    animation: gradientShift 3s ease infinite, glow 2s ease-in-out infinite alternate;
    text-shadow: 0 0 20px rgba(0, 255, 255, 0.8);
    margin: 0;
    letter-spacing: 0.1em;
  }
  @keyframes gradientShift {
    0% { background-position: 0% 50%; }
    50% { background-position: 100% 50%; }
    100% { background-position: 0% 50%; }
  }
  @keyframes glow {
    from { text-shadow: 0 0 10px #00ffff, 0 0 20px #00ffff, 0 0 30px #ff00ff; }
    to { text-shadow: 0 0 20px #00ffff, 0 0 30px #ff00ff, 0 0 40px #00ffff, 0 0 50px #ff00ff; }
  }
  .hero-subtitle {
    font-size: 1.5em;
    color: #00ffff;
    margin: 20px 0;
    opacity: 0.9;
  }
  .badge {
    display: inline-block;
    padding: 10px 20px;
    margin: 10px;
    background: linear-gradient(45deg, #1e3c72, #2a5298);
    color: white;
    border-radius: 25px;
    transition: all 0.3s ease;
    box-shadow: 0 4px 15px rgba(0,0,0,0.3);
    position: relative;
    overflow: hidden;
  }
  .badge::before {
    content: '';
    position: absolute;
    top: 0;
    left: -100%;
    width: 100%;
    height: 100%;
    background: linear-gradient(90deg, transparent, rgba(255,255,255,0.4), transparent);
    transition: left 0.5s;
  }
  .badge:hover {
    transform: scale(1.1) translateY(-5px);
    box-shadow: 0 10px 30px rgba(0, 255, 255, 0.6), 0 0 20px rgba(255, 0, 255, 0.4);
    background: linear-gradient(45deg, #00ffff, #ff00ff);
  }
  .badge:hover::before { left: 100%; }
  .card-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(300px, 1fr));
    gap: 30px;
    margin: 40px 0;
  }
  .tech-card {
    background: rgba(10, 10, 20, 0.8);
    border: 1px solid rgba(0, 255, 255, 0.3);
    border-radius: 15px;
    padding: 30px;
    text-align: center;
    transition: all 0.4s ease;
    position: relative;
    overflow: hidden;
  }
  .tech-card::before {
    content: '';
    position: absolute;
    top: -50%;
    left: -50%;
    width: 200%;
    height: 200%;
    background: linear-gradient(45deg, transparent, rgba(0,255,255,0.1), transparent);
    transform: rotate(45deg);
    transition: all 0.6s;
    opacity: 0;
  }
  .tech-card:hover {
    transform: translateY(-15px) scale(1.05);
    border-color: #00ffff;
    box-shadow: 0 20px 40px rgba(0,255,255,0.3), inset 0 0 20px rgba(0,255,255,0.1);
  }
  .tech-card:hover::before {
    opacity: 1;
    top: 0;
    left: 0;
  }
  .pipeline-svg {
    max-width: 100%;
    height: auto;
    filter: drop-shadow(0 0 20px rgba(0,255,255,0.5));
  }
  .glow-code {
    background: rgba(0,0,0,0.7);
    border: 1px solid #00ffff;
    border-radius: 10px;
    padding: 20px;
    position: relative;
    overflow: hidden;
  }
  .glow-code::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    height: 3px;
    background: linear-gradient(90deg, #00ffff, #ff00ff, #00ffff);
    background-size: 200% 100%;
    animation: shimmer 2s infinite;
  }
  @keyframes shimmer {
    0% { background-position: 200% 0; }
    100% { background-position: -200% 0; }
  }
  .tree-node {
    margin-left: 20px;
  }
  details summary {
    cursor: pointer;
    padding: 10px;
    background: rgba(0,255,255,0.1);
    border-radius: 5px;
    margin: 10px 0;
    transition: background 0.3s;
  }
  details[open] summary {
    background: rgba(255,0,255,0.2);
    box-shadow: 0 0 10px rgba(255,0,255,0.3);
  }
  table {
    width: 100%;
    border-collapse: collapse;
    background: rgba(10,10,20,0.8);
    border-radius: 10px;
    overflow: hidden;
    box-shadow: 0 10px 30px rgba(0,0,0,0.5);
  }
  th, td {
    padding: 15px;
    text-align: left;
    border-bottom: 1px solid rgba(0,255,255,0.2);
  }
  th {
    background: linear-gradient(45deg, #1e3c72, #2a5298);
    color: #00ffff;
  }
  .footer {
    text-align: center;
    padding: 40px;
    font-size: 1.2em;
    color: #00ffff;
    animation: pulse 2s infinite;
  }
  @keyframes pulse {
    0%, 100% { opacity: 0.7; }
    50% { opacity: 1; }
  }
</style>

<div class="futuristic-body">

<div class="hero-container">
  <h1 class="hero-title">TEXT2CINEMA</h1>
  <p class="hero-subtitle">Transforming Words into Cinematic Simulations</p>
  <div>
    <span class="badge">C++17</span>
    <span class="badge">OpenGL</span>
    <span class="badge">Bullet Physics</span>
    <span class="badge">FFmpeg</span>
    <span class="badge">CMake</span>
    <span class="badge">vcpkg</span>
  </div>
</div>

## Watch Text Become Cinema

Turn natural language into stunning cinematic videos. Here's a demo example:

<div class="glow-code">
```bash
text2cinema.exe "A glowing meteor falls from the sky and explodes into sparks when it hits the ocean"
