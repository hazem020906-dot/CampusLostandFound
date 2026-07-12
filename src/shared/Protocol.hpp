#ifndef PROTOCOL_HPP
#define PROTOCOL_HPP

// ============================================================
// Shared client <-> server message contract.
// Every message is one line of JSON, terminated by '\n'.
// Every message MUST have a "type" field. Below are the agreed
// type values and what fields each one carries.
// ============================================================

// ---- Client -> Server ----
// "login"           { "type": "login", "username": "...", "password": "..." }
// "register"        { "type": "register", "username": "...", "password": "...", "role": "Student"|"Security" }
// "submit_report"   { "type": "submit_report", "itemName": "...", "category": "...",
//                     "location": "...", "date": "...", "reportType": "Lost"|"Found",
//                     "imageData": "..." (base64, optional) }
// "update_status"   { "type": "update_status", "reportIndex": 0, "newStatus": "Lost"|"Found"|"Claimed"|"Returned" }
// "get_all_reports" { "type": "get_all_reports" }

// ---- Server -> Client ----
// "login_result"      { "type": "login_result", "success": true|false, "username": "...", "role": "..." }
// "register_result"   { "type": "register_result", "success": true|false, "message": "..." }
// "reports_snapshot"  { "type": "reports_snapshot", "reports": [ {itemName, category, location, date, status, imageData}, ... ] }
//                     Sent to ALL connected clients whenever the report list changes (this is what gives us live sync).
// "notification"      { "type": "notification", "message": "...", "forUsername": "..." }
//                     Sent when the matching engine finds a Lost/Found pair for that user.
// "error"             { "type": "error", "message": "..." }

#endif
