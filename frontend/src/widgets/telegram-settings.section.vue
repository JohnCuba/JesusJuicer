<script setup lang="ts">
import ContentSection from '@/components/content-section.vue'
import { useGetTelegramChatId, usePingTelegram, useSetTelegramChatId } from '@/data/telegram'

const { data: chatId, isLoading: chatIdLoading } = useGetTelegramChatId()
const setChatId = useSetTelegramChatId()

const sendPing = usePingTelegram()

const handleClickPing = () => {
  sendPing()
}

const handleSaveChatId = (event: Event) => {
  const formData = new FormData(event.target as HTMLFormElement)
  const chat_id = formData.get('chat_id')?.toString() || ''

  setChatId({
    chat_id,
  })
}
</script>

<template>
  <ContentSection id="telegram" title="Telegram bot">
    <form class="flex flex-col gap-2" @submit.prevent="handleSaveChatId">
      <input
        type="text"
        name="chat_id"
        class="input w-full"
        placeholder="Chat ID"
        :value="chatId"
        :class="{
          ['skeleton rounded-field']: chatIdLoading,
        }"
        :disabled="chatIdLoading"
      />
      <div class="flex justify-end gap-4">
        <button
          class="btn btn-dash btn-info"
          type="button"
          @click="handleClickPing"
          :disabled="!Boolean(chatId)"
        >
          ping
        </button>
        <button class="btn btn-success" type="submit">save</button>
      </div>
    </form>
  </ContentSection>
</template>
