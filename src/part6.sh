#!/bin/bash

TELEGRAM_BOT_TOKEN="6089666691:AAHQZc7Mna1l-U7sswbO7Jbc1eio1k92t7M"
TELEGRAM_USER_ID="1596955713"

URL="https://api.telegram.org/bot$TELEGRAM_BOT_TOKEN/sendMessage"
TEXT="Deploy status: $CI_JOB_STAGE%0A%0AProject:+$CI_PROJECT_NAME%0AStatus:+$CI_JOB_STATUS%0AURL:+$CI_PROJECT_URL/pipelines/$CI_PIPELINE_ID/%0ABranch:+$CI_COMMIT_REF_SLUG"

curl -s -d "chat_id=$TELEGRAM_USER_ID&disable_web_page_preview=1&text=$TEXT" $URL > /dev/null