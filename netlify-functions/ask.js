const fs = require("fs");
const path = require("path");

const stopwords = new Set([
  "the","is","are","a","an","of","to","in","on","for","and","or","can","you","i",
  "any","there","does","what","when","how","pls","please","kya","kaise","kya",
  "it","me","my","we","our","your","this","that","with","as","if","be","do","did"
]);

function clean(text) {
  return text.toLowerCase().replace(/[^a-z0-9\s]/g, " ");
}

function tokens(text) {
  return clean(text)
    .split(/\s+/)
    .filter(w => w && !stopwords.has(w));
}

function jaccard(a, b) {
  const setA = new Set(a), setB = new Set(b);
  let inter = 0;
  for (const x of setA) if (setB.has(x)) inter++;
  const uni = setA.size + setB.size - inter;
  return uni ? inter / uni : 0;
}

function loadFAQ() {
  const file = path.join(__dirname, "data", "nust_pro_dataset_1200.txt");
  const text = fs.readFileSync(file, "utf8");
  const lines = text.split(/\r?\n/);

  const faqs = [];
  let q = "";

  for (const line of lines) {
    const mQ = line.match(/^Q\d+[a-z]*:\s*(.*)$/i);
    const mA = line.match(/^A\d+[a-z]*:\s*(.*)$/i);

    if (mQ) {
      q = mQ[1].trim();
    } else if (mA && q) {
      faqs.push({
        question: q,
        answer: mA[1].trim(),
        tok: tokens(q)
      });
      q = "";
    }
  }
  return faqs;
}

const faqs = loadFAQ();

exports.handler = async (event) => {
  const q = (event.queryStringParameters?.q || "").trim();
  if (!q) {
    return { statusCode: 400, body: "Please ask a question." };
  }

  const qTok = tokens(q);
  let best = { score: 0, answer: "Sorry, I couldn't find a match." };

  for (const f of faqs) {
    const score = jaccard(qTok, f.tok);
    if (score > best.score) {
      best = { score, answer: f.answer };
    }
  }

  if (best.score < 0.12) {
    best.answer = "Sorry, I couldn't find a confident match. Try keywords like NET, scholarships, hostels, admission, GPA.";
  }

  return {
    statusCode: 200,
    headers: { "Content-Type": "text/plain" },
    body: best.answer
  };
};