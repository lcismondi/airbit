#pragma once
#include <pgmspace.h>
static const char paginaConfiguracion[] PROGMEM = R"AIRBITHTML(<!doctype html>
<html lang="es">
<head>
<meta charset="utf-8"><meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#edf4f7">
<title>Airbit · Tu espacio</title>
<style>
:root{color-scheme:light;--ink:#183540;--muted:#586f7b;--line:#dce9ed;--mint:#66d4b9;--aqua:#62cfe0;--bg:#edf4f7;--surface:#ffffffd9;--radius:27px}
*{box-sizing:border-box}html{scroll-behavior:smooth}body{margin:0;min-height:100vh;font-family:ui-rounded,"SF Pro Rounded",-apple-system,BlinkMacSystemFont,"Segoe UI",sans-serif;color:var(--ink);background:radial-gradient(ellipse at 8% 0%,#dbf3ef80,transparent 48%),radial-gradient(ellipse at 96% 90%,#d5edf680,transparent 48%),var(--bg);font-size:15px;-webkit-font-smoothing:antialiased}
button,input{font:inherit}button{cursor:pointer}button:disabled{cursor:default;opacity:.55}svg{display:block;flex-shrink:0}button,a,input{-webkit-tap-highlight-color:transparent}button{touch-action:manipulation}
.shell{max-width:1190px;margin:0 auto;padding:38px 34px 28px}.top{display:flex;align-items:center;justify-content:space-between;gap:18px;margin-bottom:43px}
.brand{display:flex;flex-direction:column;align-items:flex-start;gap:0;flex-shrink:0}.brand-logo{display:block;width:174px;height:auto}.brand small{display:block;margin-top:10px;font-size:12px;color:var(--muted);letter-spacing:.1px}.badge{display:flex;align-items:center;gap:8px;border:1px solid #ffffffc9;background:#ffffff85;border-radius:99px;padding:9px 13px;font-size:12px;color:#536e79;white-space:nowrap}.badge::before{content:"";width:6px;height:6px;border-radius:50%;background:#57bca5}
.layout{display:grid;grid-template-columns:184px minmax(0,1fr);gap:35px}aside{align-self:start;padding-top:4px}.nav{display:flex;flex-direction:column;gap:8px}.nav-label{margin:0 14px 16px;color:var(--muted);font-size:10px;letter-spacing:1.8px;font-weight:650}
.nav button{display:flex;gap:12px;align-items:center;width:100%;text-align:left;border:1px solid transparent;background:transparent;color:var(--muted);padding:14px 13px;border-radius:18px;font-size:13px;font-weight:550;transition:background .2s,color .2s,box-shadow .2s}
.nav button[aria-selected=true]{background:linear-gradient(120deg,#fff,#e2f5ef);color:var(--ink);border-color:#fff;box-shadow:0 7px 22px #3866700d}.nav button:hover{background:#ffffff90}.nav .symbol{width:22px;height:22px;display:grid;place-items:center}.nav .symbol svg{width:21px;height:21px}
.ap-box{margin-top:37px;padding:18px 16px;border:1px solid #fff;border-radius:22px;font-size:12px;line-height:1.8;background:linear-gradient(150deg,#ffffff80,#e1f4f099);color:var(--muted)}.ap-box .ap-mark{width:31px;height:31px;display:grid;place-items:center;background:#ffffffb0;border-radius:11px;color:#439e92;margin-bottom:12px}.ap-box strong{display:block;color:var(--ink);font-weight:600;font-size:12px;margin-bottom:4px}.ap-box span{overflow-wrap:anywhere}.ap-box .ap-address{font-size:11px}.ap-box p{font-size:11px;line-height:1.6;margin:11px 0 0}
main{min-width:0}.section[hidden]{display:none!important}.section-heading{margin:0 0 25px}.eyebrow{font-size:10px;text-transform:uppercase;letter-spacing:1.65px;font-weight:650;color:var(--muted)}h1{font-size:33px;line-height:1.18;letter-spacing:-1.1px;margin:8px 0 10px;font-weight:600}.lead{color:var(--muted);font-size:14px;line-height:1.7;margin:0;max-width:530px}
p{line-height:1.7}.card{background:var(--surface);border:1px solid #ffffff;border-radius:var(--radius);padding:27px;box-shadow:0 12px 34px #31515f06}.card h2{font-size:19px;letter-spacing:-.45px;font-weight:600;margin:0 0 9px}.card>p{font-size:13px;color:var(--muted);margin:0 0 23px}
.connection-grid{display:grid;grid-template-columns:minmax(0,.88fr) minmax(0,1.12fr);gap:19px;align-items:stretch}.connection-card{padding:27px 23px 22px;text-align:center;display:flex;flex-direction:column;align-items:center;justify-content:space-between;background:linear-gradient(165deg,#f8fbfdd9,#e4f2f1b0);overflow:hidden}
.orb{position:relative;width:234px;height:234px;max-width:100%;border:1px solid #ffffffce;border-radius:50%;display:flex;align-items:center;justify-content:center;flex-direction:column;background:radial-gradient(circle at 35% 25%,#a0eadab3,transparent 60%),linear-gradient(135deg,#7ecbddb3,#71e2cdbb);box-shadow:0 0 32px 12px #8cdece50,inset 0 0 35px #e5fffa75;margin:32px auto 28px;isolation:isolate}
.orb::before{content:"";position:absolute;inset:-15px;border:1px solid #ffffff60;border-radius:50%;z-index:-1}.orb::after{content:"";position:absolute;inset:-2px;border:2px solid transparent;border-top-color:#ffffffd9;border-right-color:#ffffffa0;border-radius:50%;transform:rotate(-28deg);pointer-events:none}
.orb-icon{width:57px;height:57px;margin-bottom:15px;color:#fff;filter:drop-shadow(0 2px 6px #229ca525)}.orb-title{font-size:27px;font-weight:550;color:#174a50;letter-spacing:-.6px}.orb-caption{font-size:11px;margin-top:6px;color:#235a63}.orb .orb-number{font-size:58px;font-weight:350;letter-spacing:-2px;line-height:1.1;color:#143e43;font-variant-numeric:tabular-nums}.orb .orb-unit{font-size:11px;letter-spacing:.8px;color:#285a5e;margin-top:8px}
.connection-card .eyebrow{margin:0}.network{width:100%;padding:20px 17px 17px;border-radius:21px;background:#ffffff9c;border:1px solid #fff;min-height:118px}.network strong{display:block;margin:8px 0 3px;font-size:17px;line-height:1.35;font-weight:600;overflow-wrap:anywhere;letter-spacing:-.25px}.network .hint{margin-top:4px;font-size:11px}.state{display:inline-flex;align-items:center;justify-content:center;font-size:11px;border-radius:99px;padding:7px 12px;background:#e5edf2;color:#566e7f;margin-bottom:18px;min-height:29px;max-width:100%;overflow-wrap:anywhere}.state.ok{background:#d6f4e8;color:#256e59}.state.ok::before{content:"";width:5px;height:5px;background:#3ba778;border-radius:50%;margin-right:6px}
.form-card{padding:28px}.form-card .intro{font-size:13px;color:var(--muted);margin:0 0 26px;line-height:1.7}.group{margin-bottom:21px}label.field{display:block;font-size:12px;font-weight:600;margin-bottom:9px}.input{width:100%;min-height:49px;border:1px solid #d6e5e9;border-radius:15px;background:#f6f9fa;padding:12px 14px;color:var(--ink);outline:none;font-size:14px;transition:box-shadow .2s,border-color .2s}.input::placeholder{color:#8499a4;font-size:12px}.input:focus{border-color:#57bcb1;box-shadow:0 0 0 3px #63cbb225;background:#fff}.input:disabled{background:#edf1f3;color:#93a6ae}
.hint{font-size:11px;line-height:1.65;color:var(--muted);margin-top:8px}.check{display:flex;align-items:flex-start;gap:9px;font-size:12px;line-height:1.6;color:var(--muted);margin:12px 0}.check input{accent-color:#319d86;width:15px;height:15px;margin:2px 0 0;flex-shrink:0}
.actions{display:flex;flex-direction:column;gap:8px;margin-top:25px}.primary{display:flex;align-items:center;justify-content:center;gap:12px;border:1px solid #173642;background:#173642;color:white;border-radius:16px;font-size:13px;font-weight:550;padding:14px 18px;min-height:49px;box-shadow:0 6px 14px #17364212;transition:background .2s,box-shadow .2s}.primary svg{width:16px;height:16px}.primary:hover:not(:disabled){background:#234b55;box-shadow:0 6px 20px #17364225}.actions .hint{margin:0;text-align:center;font-size:10px}
.message{min-height:46px;font-size:12px;line-height:1.65;padding-top:13px;overflow-wrap:anywhere}.message.error{color:#a63045}.message.ok{color:#27775a}
.note{display:flex;align-items:flex-start;gap:12px;border:1px solid #ffffff9c;background:#ffffff56;border-radius:20px;padding:17px 20px;color:var(--muted);font-size:12px;line-height:1.7;margin-top:18px}.note>svg{width:18px;height:18px;margin-top:1px;color:#6f949b}.note strong{color:#456773;font-weight:550}.note div{min-width:0}.note p{margin:0}.note .note-title{display:block;font-size:12px;margin-bottom:3px}
.dashboard-card{display:grid;grid-template-columns:minmax(0,1fr) minmax(0,1fr);gap:35px;align-items:center;padding:34px;background:linear-gradient(145deg,#fff9,#f9fcfce6)}.reading-visual{text-align:center;min-width:0}.reading-visual .orb{margin-top:28px;margin-bottom:34px;width:230px;height:230px}.orb.mint{background:radial-gradient(circle at 35% 25%,#c2f3daaa,transparent 65%),linear-gradient(135deg,#8ce2c3,#61d9bd);box-shadow:0 0 34px 10px #7ee0be55,inset 0 0 35px #e8fff28c}.orb.mint .orb-number{font-size:46px;letter-spacing:-1.5px}.reading-title{font-size:12px;color:var(--muted);margin:0}
.metrics{display:grid;grid-template-columns:repeat(2,minmax(0,1fr));gap:12px}.metric{padding:18px;background:#f1f7f8;border:1px solid #e5eff0;border-radius:19px;min-width:0}.metric span{display:block;font-size:11px;line-height:1.5;color:var(--muted);margin-bottom:9px}.metric strong{font-size:28px;font-weight:500;letter-spacing:-.8px;line-height:1.25;font-variant-numeric:tabular-nums}.metric small{font-size:11px;color:var(--muted);margin-left:5px}.metric.full{grid-column:1/-1}.dashboard-details{min-width:0}.dashboard-details h2{margin-bottom:12px}.dashboard-details>p{color:var(--muted);font-size:13px;margin:0 0 23px}.cal-state{min-height:45px}.cal-steps{display:flex;flex-direction:column;gap:17px;margin-top:25px}.step{display:flex;align-items:flex-start;gap:11px;font-size:12px;line-height:1.6;color:var(--muted)}.step-num{display:grid;place-items:center;width:25px;height:25px;flex-shrink:0;background:#e6f3ee;border-radius:9px;font-size:11px;color:#4c8a77}.step strong{display:block;font-size:12px;font-weight:600;color:var(--ink)}
.light-card{padding:31px;text-align:center;background:linear-gradient(150deg,#ffffffc9,#e8f6f3aa);overflow:hidden}.light-card>p{max-width:480px;margin:0 auto 17px}.light-visual{position:relative;width:235px;height:235px;margin:34px auto 30px;border:1px solid #fff;border-radius:50%;padding:18px;background:#ffffff38;box-shadow:0 0 42px #73dcd12a}.light-visual::before{content:"";position:absolute;inset:-13px;border:1px solid #ffffff90;border-radius:50%}.color-sample{width:100%;height:100%;border-radius:50%;background:#e4eef2;transition:background-color 1.5s;box-shadow:inset 0 0 45px #ffffff5c;display:grid;place-items:center}.color-sample svg{width:51px;height:51px;color:#fff;filter:drop-shadow(0 2px 7px #172f4938)}.light-state{display:block;min-height:34px;font-size:12px;color:#536f7a}.light-card .light-caption{font-size:10px;letter-spacing:1.4px;text-transform:uppercase;color:var(--muted);margin:0 0 3px}
.scale-block{margin-top:22px}.gradient{height:12px;background:linear-gradient(90deg,#2196F3,#26B6D4,#2DBD85,#65C450,#ACD442,#F2D43A,#F6B832,#F79332,#EF6C2F,#E64932,#D92D35);border-radius:99px;opacity:.85}.scale-ends{display:flex;justify-content:space-between;gap:9px;font-size:10px;color:var(--muted);margin-top:9px;line-height:1.6}.scale-info{font-size:11px;color:var(--muted);line-height:1.65;margin:20px 0 0}.footer{font-size:10px;color:var(--muted);margin:23px 0 0;min-height:34px;line-height:1.8;display:flex;align-items:center;gap:7px}.footer::before{content:"";width:4px;height:4px;border-radius:50%;background:#a1bbb9;flex-shrink:0}
.calibration-controls .control-copy{flex:1;min-width:0}.calibration-controls .primary{margin-top:15px;min-width:190px}.calibration-controls .message{min-height:52px;padding-top:11px}
:focus-visible{outline:3px solid #4faea5;outline-offset:4px}
@media(min-width:1500px){.shell{padding-top:55px}}
@media(max-width:1000px){.shell{padding:30px 25px}.layout{grid-template-columns:162px minmax(0,1fr);gap:22px}.nav button{padding:13px 10px;font-size:12px;gap:9px}.connection-grid{grid-template-columns:1fr}.connection-card{display:grid;grid-template-columns:minmax(0,1fr) minmax(0,1fr);column-gap:28px;padding:26px 23px}.connection-card>.eyebrow{grid-column:1/-1;text-align:left;margin-bottom:16px}.connection-card>.orb{grid-row:2/4;width:170px;height:170px;margin:10px auto}.connection-card>.state{align-self:end;justify-self:start;margin:0 0 12px}.connection-card>.network{align-self:start;text-align:left}.orb-icon{width:43px;height:43px;margin-bottom:12px}.orb-title{font-size:24px}.dashboard-card{gap:23px;padding:27px}.reading-visual .orb{width:185px;height:185px}.orb.mint .orb-number{font-size:36px}.orb .orb-number{font-size:46px}.metrics{grid-template-columns:1fr}.metric.full{grid-column:auto}}
@media(max-width:700px){.shell{max-width:560px;padding:25px 21px calc(112px + env(safe-area-inset-bottom))}.top{margin-bottom:32px;gap:10px}.brand-logo{width:150px}.brand small{font-size:11px}.badge{padding:8px 10px;font-size:10px}.layout{grid-template-columns:1fr;gap:0}aside{padding:0}.nav{position:fixed;z-index:10;bottom:calc(15px + env(safe-area-inset-bottom));left:50%;transform:translateX(-50%);width:calc(100% - 30px);max-width:510px;display:grid;grid-template-columns:repeat(4,minmax(0,1fr));gap:4px;border:1px solid #fff;border-radius:26px;padding:7px;background:#f8fcfafa;box-shadow:0 8px 30px #214a5620;backdrop-filter:blur(16px)}.nav-label,.ap-box{display:none}.nav button{justify-content:center;flex-direction:column;gap:6px;padding:10px 3px 9px;font-size:10px;line-height:1.3;border-radius:19px;text-align:center}.nav button[aria-selected=true]{background:linear-gradient(140deg,#e1f6f0,#dceef3);box-shadow:0 3px 8px #44716c0a;border-color:#e3f2ef}.nav .symbol{width:22px;height:22px}.nav .symbol svg{width:21px;height:21px}.nav-name{white-space:nowrap}
h1{font-size:30px;letter-spacing:-1px;line-height:1.2}.section-heading{margin-bottom:22px}.eyebrow{font-size:9px}.lead{font-size:13px;line-height:1.75}.connection-grid{gap:17px}.connection-card{display:flex;padding:25px 23px 23px}.connection-card>.eyebrow{align-self:center;margin:0}.connection-card>.orb{width:224px;height:224px;margin:30px auto 29px}.connection-card>.state{align-self:center;margin-bottom:18px}.connection-card>.network{width:100%;text-align:center;min-height:107px;padding:17px}.orb-icon{width:54px;height:54px;margin-bottom:14px}.orb-title{font-size:27px}.form-card{padding:25px 23px}.form-card .intro{margin-bottom:24px}.input{font-size:16px}.input::placeholder{font-size:12px}.actions{margin-top:23px}.note{padding:16px 17px;gap:10px;font-size:11px}.card{border-radius:25px}.dashboard-card{grid-template-columns:1fr;gap:21px;padding:27px 23px}.reading-visual .orb{width:230px;height:230px;margin:30px auto}.orb.mint .orb-number{font-size:46px}.orb .orb-number{font-size:58px}.metrics{grid-template-columns:repeat(2,minmax(0,1fr))}.metric.full{grid-column:1/-1}.metric strong{font-size:27px}.cal-steps{margin-top:23px;gap:16px}.light-card{padding:28px 23px}.light-visual{width:225px;height:225px}.scale-info{font-size:11px}.footer{margin-top:20px;font-size:10px}}
@media(max-width:700px){.calibration-controls .primary{width:100%}}
@media(max-width:360px){.shell{padding-left:16px;padding-right:16px}.brand small{font-size:10px}.badge{font-size:9px}.nav-name{font-size:9px}.orb-caption{font-size:10px}.metric{padding:15px}.metric strong{font-size:25px}}
@media(prefers-reduced-motion:reduce){html{scroll-behavior:auto}*,*::before,*::after{transition:none!important}}

.light-settings{text-align:left;border-top:1px solid var(--line);margin-top:26px;padding-top:24px}.mode-row{display:flex;align-items:center;justify-content:space-between;gap:18px}.mode-row strong{display:block;font-size:14px;font-weight:600}.mode-row .hint{margin:5px 0 0;max-width:400px}.switch{position:relative;display:inline-flex;flex-shrink:0;width:52px;height:30px}.switch input{position:absolute;inset:0;width:100%;height:100%;margin:0;opacity:0;cursor:pointer;z-index:1}.switch-track{position:absolute;inset:0;background:#cbdadd;border-radius:99px;transition:background .2s}.switch-track::after{content:"";position:absolute;left:4px;top:4px;width:22px;height:22px;border-radius:50%;background:#fff;box-shadow:0 2px 6px #203e4320;transition:transform .2s}.switch input:checked+.switch-track{background:#48bca2}.switch input:checked+.switch-track::after{transform:translateX(22px)}.switch input:focus-visible+.switch-track{outline:3px solid #4faea5;outline-offset:4px}.switch input:disabled{cursor:default}.switch input:disabled+.switch-track{opacity:.55}
.color-controls{display:grid;grid-template-columns:210px minmax(0,1fr);gap:36px;align-items:center;margin-top:27px}.color-wheel{position:relative;width:200px;height:200px;border-radius:50%;background:radial-gradient(circle,#fff 0%,#ffffff00 72%),conic-gradient(#f00,#ff0,#0f0,#0ff,#00f,#f0f,#f00);box-shadow:0 0 0 8px #ffffff9c,0 9px 25px #34758215;touch-action:none;cursor:crosshair;transition:opacity .2s}.color-wheel[aria-disabled=true]{opacity:.32;cursor:default}.wheel-marker{position:absolute;left:50%;top:50%;width:20px;height:20px;border:3px solid white;border-radius:50%;transform:translate(-50%,-50%);box-shadow:0 0 0 1px #264c5b80,0 2px 7px #254b5638;pointer-events:none}.color-label{display:flex;justify-content:space-between;align-items:center;gap:12px;margin-bottom:23px}.color-label label{font-size:12px;font-weight:600}.color-value{display:flex;align-items:center;gap:9px}.color-value input{width:35px;height:35px;padding:0;border:1px solid #d9e7e9;border-radius:10px;overflow:hidden;background:#fff;cursor:pointer}.color-value input:disabled{opacity:.45;cursor:default}.color-value input::-webkit-color-swatch-wrapper{padding:4px}.color-value input::-webkit-color-swatch{border:0;border-radius:6px}.color-value code{font:11px ui-monospace,monospace;color:var(--muted);min-width:58px}.intensity-label{display:flex;justify-content:space-between;align-items:center;gap:10px;font-size:12px;font-weight:600;margin-bottom:14px}.intensity-label output{font-size:16px;font-weight:500;font-variant-numeric:tabular-nums}.intensity-slider{width:100%;accent-color:#319d86;height:24px;margin:0;cursor:pointer}.intensity-slider:disabled{opacity:.4;cursor:default}.slider-ends{display:flex;justify-content:space-between;font-size:10px;color:var(--muted);margin-top:5px}.light-message{min-height:40px;text-align:left}.retry{border:1px solid #b9d8d2;background:#fff9;color:var(--ink);border-radius:12px;padding:9px 15px;font-size:12px}.retry[hidden]{display:none}
@media(max-width:700px){.light-card{padding:27px 23px}.light-visual{width:180px;height:180px;margin:30px auto 27px}.light-settings{margin-top:19px}.mode-row strong{font-size:13px}.color-controls{grid-template-columns:1fr;gap:27px;margin-top:30px}.color-wheel{justify-self:center;width:210px;height:210px}.mode-row .hint{font-size:11px}.color-label{margin-bottom:20px}}

.quality-controls .quality-help{margin:0 0 20px;line-height:1.7}.quality-controls .control-copy{width:100%}.quality-controls .intensity-label{justify-content:flex-end}

#section-calidad .quality-metrics{grid-template-columns:repeat(2,minmax(0,1fr))}
#section-calidad .quality-metrics small{display:block;margin:5px 0 0}
#section-calidad .quality-orb{background:#e7eef0;box-shadow:0 0 32px 10px #a1bbb970,inset 0 0 35px #ffffff15;transition:background-color 2s ease,color .3s ease,box-shadow 2s ease;color:#f8fffd}
.quality-orb .quality-title{display:block;max-width:80%;font-size:26px;font-weight:550;letter-spacing:-.6px;line-height:1.22;text-wrap:balance;text-shadow:0 1px 2px #133b4bc0,0 0 8px #133b4b66}
@media(min-width:701px) and (max-width:1000px){.quality-orb .quality-title{font-size:22px}}

.intensity-slider{appearance:none;-webkit-appearance:none;background:transparent;--intensity:50%}
.intensity-slider::-webkit-slider-runnable-track{height:6px;border-radius:99px;background:linear-gradient(90deg,#319d86 var(--intensity),#d4dfe2 var(--intensity))}
.intensity-slider::-webkit-slider-thumb{appearance:none;-webkit-appearance:none;width:17px;height:17px;margin-top:-5.5px;border-radius:50%;background:#319d86;border:2px solid #fff;box-shadow:0 1px 4px #18354025}
.intensity-slider::-moz-range-track{height:6px;border-radius:99px;background:#d4dfe2}
.intensity-slider::-moz-range-progress{height:6px;border-radius:99px;background:#319d86}
.intensity-slider::-moz-range-thumb{box-sizing:border-box;width:17px;height:17px;border-radius:50%;background:#319d86;border:2px solid #fff;box-shadow:0 1px 4px #18354025}
.quality-orb::before{border-color:#ffffffb3}
.quality-orb::after{content:none}
.device-info{display:flex;flex-wrap:wrap;gap:4px 18px;min-height:20px;margin:0;color:var(--muted);font-size:10px;line-height:1.8}.device-info strong{font-weight:600}.device-mac{font-family:ui-monospace,SFMono-Regular,Consolas,monospace;letter-spacing:.2px}
</style>
</head>
<body>
<div class="shell">
<header class="top"><div class="brand"><svg class="brand-logo" role="img" aria-label="Airbit" xmlns="http://www.w3.org/2000/svg" id="Capa_1" data-name="Capa 1" viewBox="0 0 250.94 69.95"><path d="M846.45,537.54a15.73,15.73,0,0,1,2.29-.8,19.86,19.86,0,0,1,3.09-.69c.71-.1,1.32-.13,2.54-.19.76,0,1.81-.07,3.45-.08,2.12,0,3.18,0,3.76,0a24.39,24.39,0,0,1,8.14,2.75,25.09,25.09,0,0,1,2.69,2,28.09,28.09,0,0,1,2.93,2.58,26.29,26.29,0,0,1,2.41,3c.53.73,1,1.41,1.83,2.75a21.33,21.33,0,0,1,1.49,2.41,13.43,13.43,0,0,1,.75,1.93,15.89,15.89,0,0,1,.42,2c.12.73.17,1.22.42,2.78l0,.08s.18,1.14.3,2.34a76.38,76.38,0,0,1,.21,8.33c-.14,5.79-.28,5.35-.29,8.54,0,4.23,0,6.35,0,6.42s.24,1.1,0,1.2-.42-.34-.94-.83c-1-.92-1.45-.73-2.92-1.83-.65-.49-.53-.5-1.07-.86a9.58,9.58,0,0,0-2.29-1.09c-1.76-.67-2.63-1-2.75-1a20.61,20.61,0,0,0-3.5-.38c-2.81-.22-3-.23-3.52-.23s-1.31.09-3.1.26c-2.38.22-2.74.28-3,.32a15,15,0,0,0-3,.71c-.31.12-.09.05-2.2,1l-1.71.77c-.76.35-1.13.53-1.6.77-.72.38-1.23.69-1.77,1a13.73,13.73,0,0,0-1.44.92c-1,.79-1,1.19-1.48,1.31-.36.09-.61-.05-2.19-1-1.29-.81-1.94-1.22-2.34-1.51-.92-.66-1.57-1.23-2-1.6s-.48-.45-1.75-1.64-1.43-1.29-1.78-1.67a14.74,14.74,0,0,1-1.49-1.94,26.56,26.56,0,0,1-1.38-2.35c-.49-.9-.74-1.35-1-2-.2-.46-.39-1-.74-1.94-.48-1.31-.72-2-.81-2.33a15,15,0,0,1-.3-2.75c0-.75-.11-1.84-.1-3.38a39.24,39.24,0,0,1,.19-3.94,19.88,19.88,0,0,1,.61-3.8,17.43,17.43,0,0,1,1.55-3.61,22.39,22.39,0,0,1,2.79-4.24c.53-.62,1.22-1.29,2.6-2.64a30,30,0,0,1,2.48-2.27,31.49,31.49,0,0,1,2.75-1.88A19.74,19.74,0,0,1,846.45,537.54Z M870.49,561.72 a13.51,13.51 0 1,0 -27.02,0 a13.51,13.51 0 1,0 27.02,0 Z" transform="translate(-830.68 -515.8)" fill-rule="evenodd" /><path d="M890.47,582.6V541c.06-.27.14-.57.24-.88s.25-.67.37-1,.29-.42.4-.56a6.36,6.36,0,0,1,.43-.55c.13-.14.26-.24.5-.45l.51-.41,1.14-.85,1.29-.61,1.4-.24,1.54.17,1.28.52,1,.67,1.06.86.57.7a3.55,3.55,0,0,1,.58,1,3.83,3.83,0,0,1,.23.92v31.49l-2,1.07a12.33,12.33,0,0,0-2.84,1.86c-.16.13-.65.54-1.26,1.12-.29.28-.53.52-.71.71l-1.54,1.51-1.46,1.53-1.37,1.6Z" transform="translate(-830.68 -515.8)" /><path d="M896.47,577.61" transform="translate(-830.68 -515.8)" /><circle cx="66.04" cy="6.55" r="6.55" /><circle cx="183.99" cy="6.55" r="6.55" /><path d="M1081.62,541.35a6.56,6.56,0,1,1-6.56-6.55A6.56,6.56,0,0,1,1081.62,541.35Z" transform="translate(-830.68 -515.8)" /><path d="M912.17,566.78c.05-3.69.06-6.74.06-9,0-3.65,0-5.48,0-7.57,0-1.47,0-3,0-4.42a18,18,0,0,1,.15-4.87,6.13,6.13,0,0,1,.79-2.15,5.93,5.93,0,0,1,3-2.41,7.52,7.52,0,0,1,1.61-.34,9,9,0,0,1,1.6-.06,7.44,7.44,0,0,1,1.26.18,4.53,4.53,0,0,1,2,.74,3,3,0,0,1,.92,1c.3.55.22.85.57,1.18a1.56,1.56,0,0,0,1,.37,2.48,2.48,0,0,0,1.55-.54c1.42-.9,1.52-1.15,2.32-1.61a12.84,12.84,0,0,1,2.23-.89,17.11,17.11,0,0,1,2.41-.66,17.81,17.81,0,0,1,2.27-.22,16.79,16.79,0,0,1,2.83,0,5.57,5.57,0,0,1,3.42,1.17,8.19,8.19,0,0,1,1.23,1.09,7.71,7.71,0,0,1,.69.92,6.76,6.76,0,0,1,.57,1.23,6.55,6.55,0,0,1,.23,1.06,12.12,12.12,0,0,1,.11,1.43,11.52,11.52,0,0,1,0,1.61,4.67,4.67,0,0,1-1.49,2.64,6.33,6.33,0,0,1-2.29,1.43,9.27,9.27,0,0,1-2.7.57c-1.21.13-1.3,0-2.84.12a8.16,8.16,0,0,0-2.52.42,9,9,0,0,0-2,1.27,20,20,0,0,0-2.75,2.18,11.88,11.88,0,0,0-1.61,2.17,12,12,0,0,0-1.4,3,8.9,8.9,0,0,0-.43,2.58,18.39,18.39,0,0,0,.08,1.92c.1,1.11.23,1.65-.12,2.07a1.52,1.52,0,0,1-.71.4,3.58,3.58,0,0,1-1.6.09,13.21,13.21,0,0,0-2.18,0,10,10,0,0,0-1.78.31,10.42,10.42,0,0,0-1.81.63c-.63.3-.56.36-1.34.75s-.93.33-1.6.7-1,.73-1.3.62S912.17,567.56,912.17,566.78Z" transform="translate(-830.68 -515.8)" /><path d="M925.32,581.22" transform="translate(-830.68 -515.8)" /><path d="M898.8,541.63" transform="translate(-830.68 -515.8)" /><path d="M947,596.05" transform="translate(-830.68 -515.8)" /><path d="M949.85,520.83a6.78,6.78,0,0,0-.07.84c0,.71-.06.6,0,1,0,.59,0,1,0,1.43,0,.15,0,1.31,0,1.52,0,.55,0,.59,0,1.35,0,1.11,0,1.54,0,2.45,0,.08,0,.34,0,.69v.66h0s0,.94,0,1.86c0,5-.11,8.68-.11,8.68,0,9.18.36,17.69.23,21.44a45.5,45.5,0,0,1-.11,4.82,5,5,0,0,0,0,1.33A2.5,2.5,0,0,0,950,570c.31.52.81.55,1.83,1,.14.05-.07,0,2.16,1.08,1.29.64,1.48.73,2,1a14.12,14.12,0,0,1,1.57.91c.5.35.88.66,1.44,1.12.15.12.53.45,1,.89s.7.63,1,1c.5.53.47.58.93,1.05s.52.45,1,1,.4.44,1,1l1,1c1,1,.88.85,1,1,.53.6.61.87,1,1a1.08,1.08,0,0,0,.82,0c.4-.18.4-.61.73-1.39a7.6,7.6,0,0,1,1.08-1.71c.18-.23.48-.54,1.08-1.17s.67-.67,1-1c.43-.51.4-.57.72-.89a6.14,6.14,0,0,1,1-.84c.76-.5,1.07-.49,1.08-.65s-1-.52-3.61-1.36a5.21,5.21,0,0,1-2-.92c-.33-.27-.34-.36-1-1s-.71-.57-1.07-.95-.41-.51-.9-1.07-.67-.65-1-1a8.12,8.12,0,0,1-1-1.94c-.85-1.85-.82-1.71-1-2.07a10.27,10.27,0,0,1-.6-2.34,9.74,9.74,0,0,1,.26-3.38,14.13,14.13,0,0,1,1.09-3.1A9.55,9.55,0,0,1,965,554c.16-.19.15-.16.94-1.07.62-.72.7-.82,1-1.13s.74-.76,1.06-1.06a11.54,11.54,0,0,1,1.29-1.15,8.45,8.45,0,0,1,1.75-.8,16.48,16.48,0,0,1,2.05-.72,8.8,8.8,0,0,1,2.68-.14,13.6,13.6,0,0,1,3.24.66,15.4,15.4,0,0,1,2.75,1,12.7,12.7,0,0,1,2,1.17c.23.17.64.51,1.46,1.18.58.47.85.7,1,.86a13.57,13.57,0,0,1,1.29,1.2,8.68,8.68,0,0,1,.69.92,9.51,9.51,0,0,1,1,1.92,10.36,10.36,0,0,1,.55,2.35,12.43,12.43,0,0,1,.11,2.32,12.58,12.58,0,0,1-.69,3.47,11.53,11.53,0,0,1-1.06,2.49,8.71,8.71,0,0,1-1.09,1.57c-.13.15-.41.43-1,1L985,571.08c-.57.51-.93.61-1,1a.72.72,0,0,0,.09.59c.28.34.94.15,1.66.06a9,9,0,0,1,2.47.11,11.14,11.14,0,0,1,2.94.81,15.4,15.4,0,0,1,2.08,1.09,9.7,9.7,0,0,1,1.8,1.16c.61.55.63.8,1,.9.7.17,1.43-.43,1.89-.8a5.2,5.2,0,0,0,1-1.09,11.47,11.47,0,0,0,1.11-1.94c.19-.41.51-1.07.86-2a18.15,18.15,0,0,0,1-3.47c.1-.6.13-1,.2-1.94.1-1.28.11-2.26.12-2.86,0-1.06,0-1.81-.06-2.75a19.15,19.15,0,0,0-.29-3.39c-.11-.49-.24-.92-.51-1.77-.18-.55-.42-1.29-.75-2.15a24.48,24.48,0,0,0-1.23-2.93,24.87,24.87,0,0,0-2.45-3.67c-.3-.37-.75-.89-1-1.13-.62-.73-.81-.92-.92-1-.36-.37-.39-.35-1-.87s-.41-.43-1-1L992,541l-.65-.58a11.4,11.4,0,0,0-1.06-.83,9.67,9.67,0,0,0-1.25-.75c-.87-.47-1.58-.77-1.94-.93-1.53-.66-2.3-1-3.06-1.22a20.26,20.26,0,0,0-3.9-.77,30.19,30.19,0,0,0-4.15-.32,18.4,18.4,0,0,0-4.85.4c-.8.2-.45.19-3.09,1a19.31,19.31,0,0,0-3,1c-1.2.6-1.59,1.09-2.05.91s-.55-.61-.73-2.28a36.5,36.5,0,0,1-.15-4.66c0-.29,0-1.38,0-3.55,0-3,0-3.06,0-3.9,0-1.59.06-1.75,0-2.6a8.56,8.56,0,0,0-.69-3.25,4.37,4.37,0,0,0-.59-1,4.8,4.8,0,0,0-1.4-1.11A5.62,5.62,0,0,0,958,516a5.8,5.8,0,0,0-3,0,6.32,6.32,0,0,0-1.86.77,8.58,8.58,0,0,0-1.2.92c-.15.13-.47.41-.84.8s-.39.44-.65.76a2.66,2.66,0,0,0-.31.48,3.54,3.54,0,0,0-.15.41A4.46,4.46,0,0,0,949.85,520.83Z" transform="translate(-830.68 -515.8)" /><path d="M949,520.65" transform="translate(-830.68 -515.8)" /><path d="M987.43,575.53" transform="translate(-830.68 -515.8)" /><path d="M1008.58,582.89V541.31c.06-.27.14-.57.24-.88a10,10,0,0,1,.37-.95c.15-.23.29-.42.4-.57a6.36,6.36,0,0,1,.43-.55c.13-.14.26-.24.5-.45l.51-.41,1.14-.85,1.29-.6,1.4-.25,1.53.17,1.29.52,1,.67,1.06.86.57.7a3.55,3.55,0,0,1,.58,1,3.33,3.33,0,0,1,.22.92V570.2q0,4.35,0,8.69a8.78,8.78,0,0,0-2-.19,13.74,13.74,0,0,0-1.94.26,10.37,10.37,0,0,0-1.46.31c-.69.24-.67.37-1.46.65-.6.22-.76.19-1.33.42a7.61,7.61,0,0,0-1,.5l-1.21.56-.93.55Z" transform="translate(-830.68 -515.8)" /><path d="M1033.32,535.21c.54-.41.35-1.34.23-3.11-.06-1,0-2.07,0-3.1,0,0,0-1.65.17-8.17a3.39,3.39,0,0,1,.43-1.69,5.37,5.37,0,0,1,1-1.12,10.16,10.16,0,0,1,1.09-.94,5.73,5.73,0,0,1,1.26-.75,5.46,5.46,0,0,1,1.21-.34,7.24,7.24,0,0,1,1.46-.12,4.72,4.72,0,0,1,2,.43,6,6,0,0,1,1.29,1,9.17,9.17,0,0,1,1,1,9.94,9.94,0,0,1,.75,1,6.49,6.49,0,0,1,.71,1.29,4.41,4.41,0,0,1,.29,1.57c.08,3.77-.06,6.45-.06,6.45-.47,8.48.12,2.49,0,4.93,0,.89-.15,1.94.47,2.41.36.27.78.21,1.52.14,1.12-.1,2.24-.09,3.35-.14a15.56,15.56,0,0,1,3.7.11,7.09,7.09,0,0,1,2,.58,5.18,5.18,0,0,1,1.92,1.49,5,5,0,0,1,1,2.12,5.81,5.81,0,0,1,.09,1.8,6.87,6.87,0,0,1-.52,2.58,7.3,7.3,0,0,1-1.2,1.81,4.29,4.29,0,0,1-1.3,1.26,4,4,0,0,1-2.1.29c-2.72,0-1.62.09-4.95,0-.52,0-1.05,0-1.57,0-1.32-.08-1.94-.3-2.25,0-.17.19-.15.43-.13.93,0,.79-.06,1.59,0,2.38.07,1.05,0,2.1.06,3.15,0,1.4,0,2.71,0,5.33l0,3.56a21.94,21.94,0,0,0,.06,3.36,7,7,0,0,0,.34,1.68,6.8,6.8,0,0,0,1,1.76,10.39,10.39,0,0,0,1.2,1.3,17.18,17.18,0,0,0,1.76,1.49,16,16,0,0,0,2.08,1.28,9.41,9.41,0,0,0,1.84.75,15.09,15.09,0,0,0,2.19.28c1.43.15,1.71.17,2.35.25a9.67,9.67,0,0,1,2.11.38,5.58,5.58,0,0,1,1.89,1.36,6.42,6.42,0,0,1,1.2,1.57,5.36,5.36,0,0,1,.58,4,3.44,3.44,0,0,1-.86,1.74,2.68,2.68,0,0,1-1.53.74,3.4,3.4,0,0,1-2-.65,20,20,0,0,1-1.66-1.12,16.6,16.6,0,0,0-1.74-1.07c-.6-.32-1-.5-1.83-.83s-1.44-.55-2-.74l-1.89-.63c-1.53-.52-1.55-.56-2.26-.77-.19-.05-1-.28-2-.51s-1.66-.31-2.26-.4-1.2-.16-2.16-.27c-1.39-.16-2.09-.23-2.7-.29a5.93,5.93,0,0,1-2.35-.37,4.29,4.29,0,0,1-1.29-1.06,6,6,0,0,1-1-1.55,12.4,12.4,0,0,1-.68-2.75c-.19-.93-.25-1.15-.38-1.86a49.83,49.83,0,0,1-.51-7.36c-.08-3.26-.12-4.89-.1-6.09,0-1.4-.06-2.81-.05-4.22,0-2.07.12-3.76-.76-4.2-.09,0-.09,0-.47-.12a12.41,12.41,0,0,1-2.58-.83,6.64,6.64,0,0,1-2-1.36,6.74,6.74,0,0,1-1.28-1.92,5.46,5.46,0,0,1-.56-3.25,5.78,5.78,0,0,1,1.29-2.57,8,8,0,0,1,1.43-1.49,6.72,6.72,0,0,1,1.84-1.13,7,7,0,0,1,1.34-.4C1032.81,535.23,1033,535.45,1033.32,535.21Z" transform="translate(-830.68 -515.8)" /></svg><small>Tu aire. Tu luz. Tu espacio.</small></div><span class="badge">Acceso local</span></header>
<div class="layout">
<aside><nav class="nav" aria-label="Configuración"><p class="nav-label">TU AIRBIT</p>
<button type="button" data-tab="calidad" aria-selected="true"><span class="symbol" aria-hidden="true"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.65" stroke-linecap="round"><path d="M3 7h11c5 0 5-5 2-5-1.5 0-2 1-2 2M3 12h18M3 17h9c5 0 5 5 2 5-1.5 0-2-1-2-2"/></svg></span><span class="nav-name">Calidad de aire</span></button>
<button type="button" data-tab="iluminacion" aria-selected="false" aria-label="Iluminación con color"><span class="symbol" aria-hidden="true"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.65" stroke-linecap="round"><path d="M8 15c-4-4-1.5-10 4-10s8 6 4 10l-1 2H9l-1-2zM9 20h6M12 1v1M3 5l1 1M20 6l1-1M2 12h1M21 12h1"/></svg></span><span class="nav-name">Iluminación</span></button>
<button type="button" data-tab="calibracion" aria-selected="false"><span class="symbol" aria-hidden="true"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.65" stroke-linecap="round"><circle cx="12" cy="12" r="7"/><circle cx="12" cy="12" r="2.5"/><path d="M12 2v3M12 19v3M2 12h3M19 12h3"/></svg></span><span class="nav-name">Calibración</span></button>
<button type="button" data-tab="wifi" aria-selected="false"><span class="symbol" aria-hidden="true"><svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.65" stroke-linecap="round"><path d="M3 8a14 14 0 0 1 18 0M6.5 11.5a9 9 0 0 1 11 0M10 15a3.5 3.5 0 0 1 4 0"/><circle cx="12" cy="18.5" r=".9" fill="currentColor" stroke="none"/></svg></span><span class="nav-name">Wi-Fi</span></button>
</nav><div class="ap-box"><div class="ap-mark" aria-hidden="true"><svg width="19" height="19" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"><path d="M5 9a11 11 0 0 1 14 0M8 12a6 6 0 0 1 8 0M11 15h2"/><circle cx="12" cy="18" r=".8" fill="currentColor"/></svg></div><strong>Siempre a mano</strong><span id="ap-ssid">Airbit</span><br><span class="ap-address" id="ap-ip">192.168.4.1</span><p>Tu red de configuración permanece disponible.</p></div></aside>
<main>
<section class="section" id="section-wifi" hidden>
<div class="section-heading"><span class="eyebrow">Un vínculo con tu espacio</span><h1>Conexión Wi-Fi</h1><p class="lead">Conecta Airbit a tu red y deja todo listo. Puedes volver a este espacio cuando lo necesites.</p></div>
<div class="connection-grid">
<div class="card connection-card"><span class="eyebrow">Cerca de ti, sin cables</span>
<div class="orb" aria-hidden="true"><svg class="orb-icon" viewBox="0 0 64 64" fill="none" stroke="currentColor" stroke-width="2.3" stroke-linecap="round"><path d="M9 23a37 37 0 0 1 46 0M18 32a23 23 0 0 1 28 0M27 41a8 8 0 0 1 10 0"/><circle cx="32" cy="50" r="2" fill="currentColor" stroke="none"/></svg><span class="orb-title">Wi-Fi</span><span class="orb-caption">Tu conexión con Airbit</span></div>
<span id="wifi-state" class="state">Consultando</span>
<div class="network"><span class="eyebrow">Red configurada</span><strong id="wifi-ssid">—</strong><div class="hint" id="wifi-ip">Sin dirección IP</div></div></div>
<div class="card form-card"><h2>Un lugar para conectar</h2><p class="intro">Introduce los datos de tu red Wi-Fi. Airbit se encargará de intentar la conexión.</p>
<form id="wifi-form">
<div class="group"><label class="field" for="ssid">Nombre de la red (SSID)</label><input class="input" id="ssid" name="ssid" maxlength="32" required autocomplete="off" placeholder="Nombre de tu red Wi-Fi"><div class="hint">Una red de 2,4 GHz.</div></div>
<div class="group"><label class="field" for="password">Contraseña de la red</label><input class="input" id="password" name="password" type="password" maxlength="64" autocomplete="new-password" placeholder="Nueva contraseña, si quieres cambiarla"><div class="hint">Déjala vacía para conservar la contraseña actual.</div>
<label class="check"><input id="show-password" type="checkbox">Mostrar la contraseña que escribes</label></div>
<label class="check"><input id="open-network" type="checkbox">Esta red no tiene contraseña</label>
<div class="actions"><button class="primary" id="save-wifi" type="submit" disabled>Guardar Wi-Fi<svg aria-hidden="true" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.8" stroke-linecap="round" stroke-linejoin="round"><path d="M5 12h14M14 7l5 5-5 5"/></svg></button><span class="hint">Se conservará al apagar el equipo.</span></div>
<div class="message" id="wifi-message" role="status" aria-live="polite"></div>
</form></div></div>
<div class="note"><svg aria-hidden="true" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"><circle cx="12" cy="12" r="9"/><path d="M12 11v5M12 7h.01"/></svg><div>Al cambiar de red, puede ser necesario volver a conectarte al Wi-Fi de Airbit. Si la página pierde conexión, abre de nuevo <strong>192.168.4.1</strong>.</div></div>
</section>
<section class="section" id="section-calibracion" hidden>
<div class="section-heading"><span class="eyebrow">Una referencia para empezar</span><h1>Calibración</h1><p class="lead">Cinco minutos en aire limpio para conocer el punto de partida de tu sensor MQ135.</p></div>
<div class="card dashboard-card"><div class="reading-visual"><span class="eyebrow">Tu punto de partida</span><div class="orb mint"><strong class="orb-number" id="cal-ref">—</strong><span class="orb-unit">ADC · REFERENCIA GUARDADA</span></div><p class="reading-title">Una referencia, muchos matices.</p></div>
<div class="dashboard-details"><h2>Estado de la calibración</h2><p class="cal-state" id="cal-state">Consultando el equipo.</p><div class="metrics"><div class="metric full"><span>Muestras guardadas</span><strong id="cal-count">—</strong></div></div>
<div class="cal-steps"><div class="step"><span class="step-num">1</span><div><strong>Busca aire limpio</strong>Deja que el sensor se estabilice.</div></div><div class="step"><span class="step-num">2</span><div><strong>Mantén los cuatro botones</strong>Presiónalos a la vez durante 10 segundos.</div></div><div class="step"><span class="step-num">3</span><div><strong>Dale cinco minutos</strong>Los LED respiran en verde. La referencia se guarda al terminar.</div></div></div></div></div>
<div class="note calibration-controls"><svg aria-hidden="true" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"><circle cx="12" cy="12" r="9"/><path d="M12 11v5M12 7h.01"/></svg><div class="control-copy"><strong class="note-title">Controles web</strong><p>Con el equipo en aire limpio y el sensor estabilizado, inicia cinco minutos de lecturas. Los LED respiran en verde y la referencia se guarda al terminar.</p><button class="primary" id="start-calibration" type="button" disabled>Iniciar calibración</button><div class="message" id="cal-message" role="status" aria-live="polite"></div></div></div>
</section>
<section class="section" id="section-iluminacion" hidden>
<div class="section-heading"><span class="eyebrow">La luz de tu espacio</span><h1>Iluminación con color</h1><p class="lead">Un perímetro de luz que acompaña cada cambio y cada pulsación.</p></div>
<div class="card light-card"><h2>El color de este momento</h2><p>Elige una luz para tu espacio o deja que el color siga la calidad de aire.</p><div class="light-visual"><div class="color-sample" id="light-color" aria-label="Color actual del fondo"><svg aria-hidden="true" viewBox="0 0 48 48" fill="none" stroke="currentColor" stroke-width="1.6" stroke-linecap="round"><path d="M9 17h21c9 0 9-10 3-10-3 0-4 2-4 3M9 24h30M9 31h19c9 0 9 10 3 10-3 0-4-2-4-3"/></svg></div></div><p class="light-caption">La luz que ves en Airbit</p><span class="light-state" id="light-state">Consultando el equipo.</span><div class="light-settings">
<div class="mode-row"><div><strong>Usar como iluminación</strong><p class="hint">Encendido: elige tu luz. Apagado: vuelve a la escala de calidad de aire.</p></div><label class="switch"><input id="light-manual" type="checkbox" role="switch" aria-label="Usar como iluminación" disabled><span class="switch-track" aria-hidden="true"></span></label></div>
<div class="color-controls"><div class="color-wheel" id="color-wheel" aria-hidden="true" aria-disabled="true"><span class="wheel-marker" id="wheel-marker"></span></div><div>
<div class="color-label"><label for="manual-color">Color elegido</label><div class="color-value"><input type="color" id="manual-color" value="#ffffff" disabled aria-label="Elegir color de iluminación"><code id="light-hex">#FFFFFF</code></div></div>
<label class="intensity-label" for="light-intensity"><span>Intensidad</span><output id="light-intensity-value" for="light-intensity">50 %</output></label><input class="intensity-slider" id="light-intensity" type="range" min="0" max="100" step="1" value="50" disabled><div class="slider-ends"><span>0 %</span><span>100 %</span></div>
</div></div><div class="message light-message" id="light-message" role="status" aria-live="polite"></div><button class="retry" id="light-retry" type="button" hidden>Reintentar</button>
</div></div>
</section>
<section class="section" id="section-calidad">
<div class="section-heading"><span class="eyebrow">Observa el aire a tu alrededor</span><h1>Calidad de aire</h1></div>
<div class="card dashboard-card"><div class="reading-visual"><span class="eyebrow">Tu aire ahora</span><div class="orb quality-orb" id="air-orb" role="status" aria-live="polite"><strong class="quality-title" id="air-status">Consultando</strong></div></div>
<div class="dashboard-details"><h2>Señal del sensor</h2><p>El color del equipo cambia suavemente según la señal actual y tu calibración.</p><div class="metrics quality-metrics"><div class="metric"><span>Lectura actual</span><strong id="air-adc">—</strong><small>ADC</small></div><div class="metric"><span>Índice relativo</span><strong id="air-index">—</strong><small>/ 1000</small></div></div><div class="scale-block"><div class="gradient" aria-hidden="true"></div><div class="scale-ends"><span>Aire de montaña</span><span>Toca renovar</span></div></div></div></div>
<div class="note quality-controls"><div class="control-copy"><strong class="note-title" id="quality-title">Intensidad de la luz</strong><p class="quality-help" id="quality-help">Ajusta el brillo del fondo que indica la calidad del aire.</p><div class="intensity-label"><output id="quality-intensity-value" for="quality-intensity">25 %</output></div><input class="intensity-slider" id="quality-intensity" type="range" min="0" max="100" step="1" value="25" disabled aria-labelledby="quality-title" aria-describedby="quality-help"><div class="slider-ends"><span>Apagada</span><span>100 %</span></div><p class="message light-message" id="quality-message" role="status" aria-live="polite"></p><button class="retry" id="quality-retry" type="button" hidden>Reintentar</button></div></div>
</section>
<p class="footer" id="page-status" role="status">Conectando con Airbit…</p>
<p class="device-info" aria-label="Información del dispositivo"><span>Firmware <strong id="device-version">—</strong></span><span>MAC Wi-Fi <span class="device-mac" id="device-mac">—</span></span></p>
</main></div></div>
<script>
'use strict';
const el=id=>document.getElementById(id);
let state=null, token='', initialized=false, polling=false, saving=false;
let startingCalibration=false, calibrationRequested=false, calibrationVersion=0;
document.querySelectorAll('[data-tab]').forEach(button=>button.addEventListener('click',()=>{
 document.querySelectorAll('[data-tab]').forEach(b=>b.setAttribute('aria-selected',String(b===button)));
 document.querySelectorAll('.section').forEach(s=>s.hidden=s.id!=='section-'+button.dataset.tab);
}));
el('show-password').addEventListener('change',()=>el('password').type=el('show-password').checked?'text':'password');
el('open-network').addEventListener('change',()=>el('password').disabled=el('open-network').checked);
async function request(url,options={}){
 const controller=new AbortController(), timer=setTimeout(()=>controller.abort(),4500);
 try{const response=await fetch(url,{cache:'no-store',credentials:'same-origin',...options,signal:controller.signal});const data=await response.json();if(!response.ok)throw new Error(data.error||'No se pudo completar la operación.');return data;}catch(error){if(error instanceof TypeError)throw new Error('Se perdió la conexión con Airbit. Vuelve a conectarte a su Wi-Fi y pulsa Reintentar.');throw error;}finally{clearTimeout(timer);}
}
function updateCalibrationControl(){
 const active=calibrationRequested||Boolean(state&&state.calibracion.activa);
 el('start-calibration').disabled=!token||saving||startingCalibration||active;
 el('start-calibration').textContent=startingCalibration?'Iniciando…':active?'Calibración en curso':'Iniciar calibración';
}

function updateAirQuality(data){
 const available=data.aire.escalaDisponible,orb=el('air-orb');
 const color=available?data.aire.color:[231,238,240];
 el('air-status').textContent=available?data.aire.estado:data.calibracion.activa?'Calibrando':!data.calibracion.valida?'Sin referencia':'Esperando lectura';
 orb.style.backgroundColor='rgb('+color.join(',')+')';
 orb.style.color='#f8fffd';
 orb.style.boxShadow='0 0 32px 10px rgba('+color.join(',')+',.48),inset 0 0 35px #ffffff15';
}
function showState(data,acceptLighting=true,acceptQuality=true){
 const finished=Boolean(state&&state.calibracion.activa&&!data.calibracion.activa);
 state=data;token=data.csrf;calibrationRequested=false;
 const device=data.dispositivo||{};el('device-version').textContent=device.version?'v'+device.version:'—';el('device-mac').textContent=device.mac||'—';
 if(finished){el('cal-message').className='message';el('cal-message').textContent='El proceso terminó. Revisa la referencia guardada.';}
 el('ap-ssid').textContent=data.ap.ssid;el('ap-ip').textContent=data.ap.ip;
 el('wifi-ssid').textContent=data.wifi.ssid||'Sin configurar';
 el('wifi-state').textContent=data.wifi.conectado?'Conectado':data.wifi.estado;
 el('wifi-state').className='state'+(data.wifi.conectado?' ok':'');
 el('wifi-ip').textContent=data.wifi.conectado?'IP '+data.wifi.ip:'Sin dirección IP';
 if(!initialized){el('ssid').value=data.wifi.ssid;el('open-network').checked=data.wifi.redAbierta;el('password').disabled=data.wifi.redAbierta;initialized=true;}
 el('cal-ref').textContent=data.calibracion.valida?data.calibracion.referencia.toFixed(3):'—';
 el('cal-count').textContent=data.calibracion.valida?data.calibracion.muestras:'—';
 el('cal-state').textContent=data.calibracion.activa?'Calibrando · quedan '+data.calibracion.restante+' segundos':data.calibracion.valida?'Referencia guardada':'Todavía no hay una referencia guardada.';
 el('air-adc').textContent=data.aire.disponible?data.aire.adc.toFixed(0):'—';
 el('air-index').textContent=data.aire.escalaDisponible?data.aire.indice:'—';
 updateAirQuality(data);
 if(acceptLighting&&data.iluminacion&&(!lightingInitialized||(!lightingDirty&&!lightingSending&&!lightingPending))){lightingDraft={manual:data.iluminacion.manual,color:[...data.iluminacion.color],intensidad:data.iluminacion.intensidad};lightingInitialized=true;}
 if(acceptQuality&&data.calidad&&(!qualityInitialized||(!qualityDirty&&!qualitySending&&!qualityPending))){qualityDraft=data.calidad.intensidad;qualityInitialized=true;}
 updateQualityView();updateLightingView();
 el('save-wifi').disabled=saving||startingCalibration||!token;
 updateCalibrationControl();
 el('page-status').textContent='Airbit conectado · estado actualizado';
}
async function poll(){
 if(polling||saving||startingCalibration||lightingSending||lightingPending||qualitySending||qualityPending)return;
 polling=true;const version=calibrationVersion,lightVersion=lightingVersion,scaleVersion=qualityVersion;
 try{const data=await request('/api/estado');if(version===calibrationVersion)showState(data,lightVersion===lightingVersion,scaleVersion===qualityVersion);}catch(error){if(version===calibrationVersion)el('page-status').textContent='Sin respuesta de Airbit. Reintentando…';}finally{polling=false;}
}
el('wifi-form').addEventListener('submit',async event=>{
 event.preventDefault();if(saving||startingCalibration||!token)return;
 saving=true;updateCalibrationControl();el('save-wifi').disabled=true;el('wifi-message').className='message';el('wifi-message').textContent='';
 const body=new URLSearchParams({ssid:el('ssid').value,password:el('password').value,redAbierta:el('open-network').checked?'1':'0'});
 try{const result=await request('/api/wifi',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded','X-Airbit-Token':token},body});if(result.guardado!==true)throw new Error('No se recibio confirmacion de guardado.');el('password').value='';el('wifi-message').className='message ok';el('wifi-message').textContent='Configuración guardada. Airbit intentará conectarse a tu red.';}
 catch(error){el('wifi-message').className='message error';el('wifi-message').textContent=error.name==='AbortError'?'No se recibió confirmación. Comprueba la conexión y vuelve a intentarlo.':error.message;}
 finally{saving=false;el('save-wifi').disabled=!token;updateCalibrationControl();if(lightingPending)sendLighting();else if(qualityPending)sendQuality();setTimeout(poll,800);}
});
el('start-calibration').addEventListener('click',async()=>{
 if(!token||saving||startingCalibration||calibrationRequested||(state&&state.calibracion.activa))return;
 startingCalibration=true;++calibrationVersion;updateCalibrationControl();el('save-wifi').disabled=true;
 el('cal-message').className='message';el('cal-message').textContent='';
 try{
  const result=await request('/api/calibracion',{method:'POST',headers:{'X-Airbit-Token':token}});
  if(result.iniciada!==true)throw new Error('No se recibió confirmación de inicio.');
  calibrationRequested=true;
  el('cal-message').className='message ok';
  el('cal-message').textContent='Calibración iniciada. Mantén el equipo en aire limpio durante cinco minutos.';
 }catch(error){
  el('cal-message').className='message error';
  el('cal-message').textContent=error.name==='AbortError'?'No se recibió confirmación. Consultando el estado del equipo.':error.message;
 }finally{
  startingCalibration=false;updateCalibrationControl();el('save-wifi').disabled=saving||!token;if(lightingPending)sendLighting();else if(qualityPending)sendQuality();setTimeout(poll,800);
 }
});

let lightingDraft={manual:false,color:[255,255,255],intensidad:50}, lightingInitialized=false, lightingDirty=false, lightingVersion=0, lightingPending=null, lightingSending=false, lightingTimer=null;
const colorHex=rgb=>'#'+rgb.map(v=>v.toString(16).padStart(2,'0')).join('');
const hexColor=value=>[1,3,5].map(i=>parseInt(value.slice(i,i+2),16));
function rgbHsv(rgb){
 const [r,g,b]=rgb.map(v=>v/255),max=Math.max(r,g,b),min=Math.min(r,g,b),d=max-min;
 let h=0;if(d){h=max===r?((g-b)/d)%6:max===g?(b-r)/d+2:(r-g)/d+4;h=(h*60+360)%360;}
 return {h,s:max?d/max:0};
}
function hsvRgb(h,s){
 const c=s,x=c*(1-Math.abs((h/60)%2-1)),m=1-c;
 const parts=h<60?[c,x,0]:h<120?[x,c,0]:h<180?[0,c,x]:h<240?[0,x,c]:h<300?[x,0,c]:[c,0,x];
 return parts.map(v=>Math.round((v+m)*255));
}
function updateLightingView(){
 updateQualityView();
 const manual=lightingDraft.manual,enabled=Boolean(token)&&manual;
 el('light-manual').checked=manual;el('light-manual').disabled=!token;
 el('manual-color').disabled=!enabled;el('light-intensity').disabled=!enabled;
 el('color-wheel').setAttribute('aria-disabled',String(!enabled));
 el('manual-color').value=colorHex(lightingDraft.color);el('light-hex').textContent=colorHex(lightingDraft.color).toUpperCase();
 el('light-intensity').style.setProperty('--intensity',lightingDraft.intensidad+'%');
 el('light-intensity').value=lightingDraft.intensidad;el('light-intensity-value').textContent=lightingDraft.intensidad+' %';
 const hsv=rgbHsv(lightingDraft.color),angle=hsv.h*Math.PI/180;
 el('wheel-marker').style.left=(50+Math.sin(angle)*hsv.s*48)+'%';el('wheel-marker').style.top=(50-Math.cos(angle)*hsv.s*48)+'%';
 const calibrating=Boolean(state&&state.calibracion.activa),automatic=Boolean(state&&state.aire.escalaDisponible);
 const color=manual?lightingDraft.color.map(v=>Math.floor(v*lightingDraft.intensidad/100)):automatic?state.aire.color.map(v=>Math.floor(v*qualityDraft/100)):null;
 el('light-color').style.backgroundColor=calibrating?'#00b859':color?'rgb('+color.join(',')+')':'#ecf1f8';
 el('light-state').textContent=calibrating?'Calibración · verde respirando':manual?'Iluminación manual · '+lightingDraft.intensidad+' %':automatic?'Fondo de calidad de aire activo':'Completa una calibración para mostrar el fondo de calidad.';
}
function queueLighting(guardar){
 if(!token)return;
 lightingDirty=true;++lightingVersion;clearTimeout(lightingTimer);
 lightingPending={version:lightingVersion,guardar,values:{manual:lightingDraft.manual?'1':'0',r:String(lightingDraft.color[0]),g:String(lightingDraft.color[1]),b:String(lightingDraft.color[2]),intensidad:String(lightingDraft.intensidad),guardar:guardar?'1':'0'}};
 el('light-message').className='message light-message';el('light-message').textContent='';el('light-retry').hidden=true;
 if(guardar)return sendLighting();
 lightingTimer=setTimeout(sendLighting,100);
}
async function sendLighting(){
 if(lightingSending||qualitySending||!lightingPending||saving||startingCalibration)return;
 const pending=lightingPending;lightingPending=null;lightingSending=true;
 try{
  const result=await request('/api/iluminacion',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded','X-Airbit-Token':token},body:new URLSearchParams(pending.values)});
  if(result.aplicado!==true||(pending.guardar&&result.guardado!==true))throw new Error('No se recibió confirmación de la iluminación.');
  if(pending.version===lightingVersion&&pending.guardar){
   lightingDirty=false;el('light-message').className='message light-message ok';el('light-message').textContent='Iluminación guardada.';el('light-retry').hidden=true;
  }
 }catch(error){
  if(pending.version===lightingVersion){el('light-message').className='message light-message error';el('light-message').textContent=error.name==='AbortError'?'No se recibió confirmación. Puedes reintentar.':error.message;el('light-retry').hidden=false;}
 }finally{lightingSending=false;if(qualityPending)sendQuality();else if(lightingPending)sendLighting();}
}
el('light-manual').addEventListener('change',()=>{lightingDraft.manual=el('light-manual').checked;updateLightingView();return queueLighting(true);});
el('manual-color').addEventListener('input',()=>{lightingDraft.color=hexColor(el('manual-color').value);updateLightingView();queueLighting(false);});
el('manual-color').addEventListener('change',()=>{lightingDraft.color=hexColor(el('manual-color').value);updateLightingView();return queueLighting(true);});
el('light-intensity').addEventListener('input',()=>{lightingDraft.intensidad=Number(el('light-intensity').value);updateLightingView();queueLighting(false);});
el('light-intensity').addEventListener('change',()=>{lightingDraft.intensidad=Number(el('light-intensity').value);updateLightingView();return queueLighting(true);});
el('light-retry').addEventListener('click',()=>queueLighting(true));
let wheelPointer=null;
function selectWheel(event){
 const rect=el('color-wheel').getBoundingClientRect(),x=event.clientX-rect.left-rect.width/2,y=event.clientY-rect.top-rect.height/2;
 const h=(Math.atan2(x,-y)*180/Math.PI+360)%360,s=Math.min(1,Math.hypot(x,y)/(rect.width/2));
 lightingDraft.color=hsvRgb(h,s);updateLightingView();
}
el('color-wheel').addEventListener('pointerdown',event=>{
 if(!token||!lightingDraft.manual||event.isPrimary===false)return;
 wheelPointer=event.pointerId;el('color-wheel').setPointerCapture(event.pointerId);selectWheel(event);queueLighting(false);
});
el('color-wheel').addEventListener('pointermove',event=>{if(event.pointerId!==wheelPointer)return;selectWheel(event);queueLighting(false);});
function finishWheel(event){if(event.pointerId!==wheelPointer)return;wheelPointer=null;return queueLighting(true);}
el('color-wheel').addEventListener('pointerup',finishWheel);el('color-wheel').addEventListener('pointercancel',finishWheel);el('color-wheel').addEventListener('lostpointercapture',finishWheel);

let qualityDraft=25,qualityInitialized=false,qualityDirty=false,qualityVersion=0,qualityPending=null,qualitySending=false,qualityTimer=null;
function updateQualityView(){
 el('quality-intensity').style.setProperty('--intensity',qualityDraft+'%');
 el('quality-intensity').value=qualityDraft;el('quality-intensity').disabled=!token;
 el('quality-intensity-value').textContent=qualityDraft+' %';
 el('quality-help').textContent=lightingDraft.manual?'Se aplicará al apagar «Usar como iluminación».':'Ajusta el brillo del fondo que indica la calidad del aire.';
}
function queueQuality(guardar){
 if(!token)return;
 qualityDirty=true;++qualityVersion;clearTimeout(qualityTimer);
 qualityPending={version:qualityVersion,guardar,values:{intensidad:String(qualityDraft),guardar:guardar?'1':'0'}};
 el('quality-message').className='message light-message';el('quality-message').textContent='';el('quality-retry').hidden=true;
 if(guardar)return sendQuality();
 qualityTimer=setTimeout(sendQuality,100);
}
async function sendQuality(){
 if(qualitySending||lightingSending||!qualityPending||saving||startingCalibration)return;
 const pending=qualityPending;qualityPending=null;qualitySending=true;
 try{
  const result=await request('/api/calidad',{method:'POST',headers:{'Content-Type':'application/x-www-form-urlencoded','X-Airbit-Token':token},body:new URLSearchParams(pending.values)});
  if(result.aplicado!==true||(pending.guardar&&result.guardado!==true))throw new Error('No se recibió confirmación de la intensidad.');
  if(pending.version===qualityVersion&&pending.guardar){qualityDirty=false;el('quality-message').className='message light-message ok';el('quality-message').textContent='Intensidad guardada.';el('quality-retry').hidden=true;}
 }catch(error){
  if(pending.version===qualityVersion){el('quality-message').className='message light-message error';el('quality-message').textContent=error.name==='AbortError'?'No se recibió confirmación. Puedes reintentar.':error.message;el('quality-retry').hidden=false;}
 }finally{qualitySending=false;if(lightingPending)sendLighting();else if(qualityPending)sendQuality();}
}
el('quality-intensity').addEventListener('input',()=>{qualityDraft=Number(el('quality-intensity').value);updateQualityView();updateLightingView();queueQuality(false);});
el('quality-intensity').addEventListener('change',()=>{qualityDraft=Number(el('quality-intensity').value);updateQualityView();updateLightingView();return queueQuality(true);});
el('quality-retry').addEventListener('click',()=>queueQuality(true));

poll();setInterval(poll,2500);
</script>
</body></html>
)AIRBITHTML";
