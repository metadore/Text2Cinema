<style>
  :root {
    --bg-primary: #000000;
    --bg-secondary: #0a0a0a;
    --accent-blue: #00ffff;
    --accent-purple: #ff00ff;
    --accent-cyan: #00ffaa;
    --glow-blue: rgba(0, 255, 255, 0.6);
    --glow-purple: rgba(255, 0, 255, 0.6);
  }
  body {
    background: linear-gradient(135deg, var(--bg-primary) 0%, var(--bg-secondary) 100%);
    color: #ffffff;
    font-family: 'SF Mono', 'Monaco', monospace;
    line-height: 1.6;
    margin: 0;
    padding: 20px;
  }
  .hero {
    text-align: center;
    padding: 80px 20px;
    position: relative;
    overflow: hidden;
  }
  .hero::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    bottom: 0;
    background: radial-gradient(circle at 20% 80%, var(--glow-blue) 0%, transparent 50%),
                radial-gradient(circle at 80% 20%, var(--glow-purple) 0%, transparent 50%),
                radial-gradient(circle at 40% 40%, var(--accent-cyan) 0%, transparent 50%);
    animation: heroFloat 6s ease-in-out infinite;
    z-index: 0;
  }
  @keyframes heroFloat {
    0%, 100% { transform: scale(1) rotate(0deg); opacity: 0.4; }
    50% { transform: scale(1.1) rotate(1deg); opacity: 0.8; }
  }
  .hero-content {
    position: relative;
    z-index: 1;
  }
  .hero-title {
    font-size: clamp(3rem, 8vw, 6rem);
    font-weight: 700;
    background: linear-gradient(45deg, var(--accent-blue), var(--accent-purple), var(--accent-blue), var(--accent-cyan));
    background-size: 300% 300%;
    -webkit-background-clip: text;
    background-clip: text;
    -webkit-text-fill-color: transparent;
    text-fill-color: transparent;
    animation: gradientFlow 4s ease infinite, titleGlow 2s ease-in-out infinite alternate;
    margin: 0 0 1rem 0;
    letter-spacing: 0.05em;
  }
  @keyframes gradientFlow {
    0% { background-position: 0% 50%; }
    50% { background-position: 100% 50%; }
    100% { background-position: 0% 50%; }
  }
  @keyframes titleGlow {
    0% { 
      text-shadow: 0 0 20px var(--glow-blue), 0 0 40px var(--glow-purple), 0 0 60px var(--glow-blue);
    }
    100% { 
      text-shadow: 0 0 30px var(--glow-blue), 0 0 50px var(--glow-purple), 0 0 70px var(--glow-blue), 0 0 100px var(--glow-purple);
    }
  }
  .hero-subtitle {
    font-size: 1.5rem;
    color: var(--accent-blue);
    margin: 0 0 3rem 0;
    opacity: 0.9;
    animation: fadeInUp 1s ease 0.5s both;
  }
  .badges {
    display: flex;
    flex-wrap: wrap;
    gap: 1rem;
    justify-content: center;
    animation: fadeInUp 1s ease 1s both;
  }
  .badge {
    padding: 0.8rem 1.5rem;
    background: linear-gradient(135deg, rgba(30,60,114,0.8), rgba(42,82,152,0.8));
    border: 1px solid var(--accent-blue);
    border-radius: 50px;
    color: white;
    font-size: 0.9rem;
    font-weight: 500;
    transition: all 0.4s cubic-bezier(0.25, 0.46, 0.45, 0.94);
    position: relative;
    overflow: hidden;
  }
  .badge::before {
    content: '';
    position: absolute;
    inset: 0;
    background: linear-gradient(90deg, transparent, rgba(255,255,255,0.3), transparent);
    transform: translateX(-100%);
    transition: transform 0.6s;
  }
  .badge:hover {
    transform: translateY(-8px) scale(1.05);
    box-shadow: 0 20px 40px var(--glow-blue), 0 0 30px var(--glow-purple);
    border-color: var(--accent-purple);
    background: linear-gradient(135deg, var(--accent-blue), var(--accent-purple));
  }
  .badge:hover::before {
    transform: translateX(100%);
  }
  .section {
    max-width: 1200px;
    margin: 0 auto 4rem auto;
  }
  .section-title {
    font-size: 2.5rem;
    background: linear-gradient(45deg, var(--accent-blue), var(--accent-purple));
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    margin-bottom: 2rem;
    position: relative;
  }
  .section-title::after {
    content: '';
    position: absolute;
    bottom: -10px;
    left: 0;
    width: 60px;
    height: 3px;
    background: linear-gradient(90deg, var(--accent-blue), var(--accent-purple));
    border-radius: 2px;
    animation: lineGlow 2s ease-in-out infinite alternate;
  }
  @keyframes lineGlow {
    from { box-shadow: 0 0 5px var(--accent-blue); }
    to { box-shadow: 0 0 15px var(--accent-purple); }
  }
  .demo-command {
    background: rgba(0,0,0,0.7);
    border: 2px solid var(--accent-blue);
    border-radius: 15px;
    padding: 2rem;
    margin: 2rem 0;
    position: relative;
    overflow: hidden;
  }
  .demo-command::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    height: 4px;
    background: linear-gradient(90deg, var(--accent-blue), var(--accent-purple), var(--accent-blue));
    background-size: 200% 100%;
    animation: shimmer 3s infinite;
  }
  @keyframes shimmer {
    0% { background-position: 200% 0; }
    100% { background-position: -200% 0; }
  }
  pre {
    background: rgba(10,10,20,0.9) !important;
    border: 1px solid rgba(0,255,255,0.3) !important;
    border-radius: 10px !important;
    padding: 1.5rem !important;
    box-shadow: 0 10px 30px rgba(0,0,0,0.5) !important;
  }
  .cards-grid {
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(350px, 1fr));
    gap: 2rem;
    margin-top: 3rem;
  }
  .card {
    background: linear-gradient(145deg, rgba(20,20,40,0.8), rgba(10,10,25,0.9));
    border: 1px solid rgba(0,255,255,0.2);
    border-radius: 20px;
    padding: 2.5rem 2rem;
    text-align: center;
    transition: all 0.5s cubic-bezier(0.23, 1, 0.320, 1);
    position: relative;
    overflow: hidden;
  }
  .card::before {
    content: '';
    position: absolute;
    top: 0;
    left: 0;
    right: 0;
    height: 4px;
    background: linear-gradient(90deg, transparent, var(--accent-blue), var(--accent-purple), transparent);
    opacity: 0;
    transition: opacity 0.4s;
  }
  .card:hover {
    transform: translateY(-15px) scale(1.02);
    border-color: var(--accent-blue);
    box-shadow: 0 30px 60px var(--glow-blue), inset 0 0 30px rgba(0,255,255,0.1);
  }
  .card:hover::before {
    opacity: 1;
  }
  .card-icon {
    font-size: 4rem;
    margin-bottom: 1rem;
    background: linear-gradient(45deg, var(--accent-blue), var(--accent-purple));
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    display: block;
  }
  .tree {
    background: rgba(0,0,0,0.6);
    border-radius: 10px;
    padding: 2rem;
    font-family: monospace;
    font-size: 0.95rem;
    border-left: 4px solid var(--accent-blue);
  }
  details {
    margin: 1rem 0;
  }
  summary {
    padding: 1rem;
    background: rgba(0,255,255,0.1);
    border-radius: 8px;
    cursor: pointer;
    transition: all 0.3s ease;
    font-weight: 500;
  }
  summary:hover {
    background: rgba(0,255,255,0.2);
    box-shadow: 0 0 15px var(--glow-blue);
  }
  details[open] summary {
    background: linear-gradient(135deg, rgba(0,255,255,0.2), rgba(255,0,255,0.2));
    border-radius: 8px 8px 0 0;
  }
  table {
    width: 100%;
    border-collapse: collapse;
    background: rgba(15,15,35,0.9);
    border-radius: 15px;
    overflow: hidden;
    box-shadow: 0 20px 40px rgba(0,0,0,0.6);
    margin: 2rem 0;
  }
  th {
    background: linear-gradient(135deg, #1e3c72, #2a5298);
    color: var(--accent-blue);
    padding: 1.5rem;
    text-align: left;
    font-weight: 600;
  }
  td {
    padding: 1.5rem;
    border-bottom: 1px solid rgba(0,255,255,0.1);
  }
  tr:hover td {
    background: rgba(0,255,255,0.05);
  }
  .footer {
    text-align: center;
    padding: 4rem 2rem;
    border-top: 1px solid rgba(0,255,255,0.2);
    margin-top: 4rem;
  }
  .footer-text {
    font-size: 2rem;
    background: linear-gradient(45deg, var(--accent-purple), var(--accent-cyan));
    -webkit-background-clip: text;
    -webkit-text-fill-color: transparent;
    animation: gradientFlow 3s ease infinite;
    margin: 0;
  }
  @keyframes fadeInUp {
    from {
      opacity: 0;
      transform: translateY(30px);
    }
    to {
      opacity: 1;
      transform: translateY(0);
    }
  }
</style>

<div style="max-width: 1200px; margin: 0 auto;">

<!-- Hero Banner -->
<div class="hero">
  <div class="hero-content">
    <h1 class="hero-title">TEXT2CINEMA</h1>
    <p class="hero-subtitle">Transforming Words into Cinematic Simulations</p>
    <div class="badges">
      <span class="badge">C++17</span>
      <span class="badge">OpenGL</span>
      <span class="badge">Bullet Physics</span>
      <span class="badge">FFmpeg</span>
      <span class="badge">CMake</span>
      <span class="badge">vcpkg</span>
    </div>
  </div>
</div>

<!-- Demo Section -->
<div class="section">
  <h2 class="section-title">Watch Text Become Cinema</h2>
  <p style="text-align: center; font-size: 1.2rem; opacity: 0.9; max-width: 800px; margin: 0 auto 3rem;">
    Witness natural language transform into fully simulated cinematic scenes. No models, no textures—just pure mathematics, physics, and procedural generation.
  </p>
  
  <div style="display: grid; grid-template-columns: 1fr 1fr; gap: 3rem; align-items: center;">
    <div>
      <div class="demo-command">
        <pre><code>text2cinema.exe "A glowing meteor falls from the sky and explodes into sparks when it hits the ocean"</code></pre>
      </div>
    </div>
    <div style="text-align: center;">
      <p style="font-size: 1.1rem; margin-bottom: 1rem;">🌀 Generates:</p>
      <div style="background: rgba(0,0,0,0.7); padding: 1rem; border-radius: 10px; border: 1px solid var(--accent-blue);">
        <code>generated/output_video.mp4</code><br>
        <code>generated/GeneratedScene.cpp</code>
      </div>
    </div>
  </div>
</div>

<!-- Architecture Pipeline -->
<div class="section">
  <h2 class="section-title">Architecture Pipeline</h2>
  <svg viewBox="0 0 1000 400" style="width: 100%; height: auto; filter: drop-shadow(0 10px 30px var(--glow-blue));">
    <defs>
      <filter id="glowFilter">
        <feGaussianBlur stdDeviation="4" result="blur"/>
        <feMerge>
          <feMergeNode in="blur"/>
          <feMergeNode in="SourceGraphic"/>
        </feMerge>
      </filter>
      <linearGradient id="arrowGradient" x1="0%" y1="0%" x2="100%" y2="0%">
        <stop offset="0%" stop-color="var(--accent-blue)"/>
        <stop offset="50%" stop-color="var(--accent-purple)"/>
        <stop offset="100%" stop-color="var(--accent-blue)"/>
      </linearGradient>
      <marker id="arrowhead" markerWidth="10" markerHeight="7" refX="9" refY="3.5" orient="auto">
        <polygon points="0 0, 10 3.5, 0 7" fill="url(#arrowGradient)"/>
      </marker>
    </defs>
    
    <!-- Pipeline Nodes -->
    <g filter="url(#glowFilter)">
      <rect x="20" y="30" width="120" height="50" rx="10" fill="rgba(26,26,46,0.9)" stroke="var(--accent-blue)" stroke-width="2"/>
      <text x="80" y="60" text-anchor="middle" fill="var(--accent-blue)" font-size="12" font-weight="bold">Text Input</text>
      
      <rect x="20" y="120" width="120" height="50" rx="10" fill="rgba(26,26,46,0.9)" stroke="var(--accent-purple)" stroke-width="2">
        <animate attributeName="opacity" values="0.7;1;0.7" dur="2s" repeatCount="indefinite"/>
      </rect>
      <text x="80" y="150" text-anchor="middle" fill="var(--accent-purple)" font-size="12" font-weight="bold">TextParser</text>
      
      <rect x="20" y="210" width="120" height="50" rx="10" fill="rgba(26,26,46,0.9)" stroke="#00ff80" stroke-width="2">
        <animate attributeName="opacity" values="1;0.7;1" dur="2s" repeatCount="indefinite" begin="0.3s"/>
      </rect>
      <text x="80" y="240" text-anchor="middle" fill="#00ff80" font-size="12" font-weight="bold">GeneratedScene</text>
      
      <rect x="20" y="300" width="120" height="50" rx="10" fill="rgba(26,26,46,0.9)" stroke="#ffaa00" stroke-width="2">
        <animate attributeName="opacity" values="0.7;1;0.7" dur="2s" repeatCount="indefinite" begin="0.6s"/>
      </rect>
      <text x="80" y="330" text-anchor="middle" fill="#ffaa00" font-size="12" font-weight="bold">Physics Engine</text>
      
      <!-- Middle nodes -->
      <rect x="200" y="90" width="120" height="50" rx="10" fill="rgba(26,26,46,0.9)" stroke="#ff4488" stroke-width="2">
        <animate attributeName="opacity" values="1;0.7;1" dur="2s" repeatCount="indefinite" begin="0.9s"/>
      </rect>
      <text x="260" y="120" text-anchor="middle" fill="#ff4488" font-size="12" font-weight="bold">Particle System</text>
      
      <rect x="200" y="220" width="120" height="50" rx="10" fill="rgba(26,26,46,0
