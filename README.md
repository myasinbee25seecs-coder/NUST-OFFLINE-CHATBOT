# NUST FAQ Chatbot

A web-based FAQ chatbot for NUST admissions and student queries.

## Features
- Browser chatbot UI
- Netlify deployment
- FAQ dataset-based answer retrieval
- C++ matcher included in the repository
- JavaScript/Netlify function demo for web hosting

## Files
- `public/index.html` — frontend
- `netlify-functions/ask.js` — Netlify function
- `data/nust_pro_dataset_1200.txt` — FAQ dataset
- `cpp/faq_matcher.cpp` — C++ matching logic
- `netlify.toml` — Netlify config

## Run C++ version locally
```bash
g++ cpp/faq_matcher.cpp -o faq
./faq