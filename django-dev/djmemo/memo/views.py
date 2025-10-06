import markdown
from markdown.extensions import nl2br
from django.shortcuts import get_object_or_404
from django.views.generic import CreateView, DetailView, ListView, UpdateView
from django.urls import reverse_lazy
from django.utils import timezone
from .models import Tag, Memo
from .forms import MemoForm, TagForm

class HomeView(ListView):
    model = Memo
    template_name = 'memo/home.html'
    context_object_name = 'memos'
    ordering = ['-pub_date']

class MemoCreateView(CreateView):
    model = Memo
    form_class = MemoForm
    template_name = 'memo/add_memo.html'
    success_url = reverse_lazy('home')

    def form_valid(self, form):
        form.instance.pub_date = timezone.now()
        return super().form_valid(form)

class MemoUpdateView(UpdateView):
    model = Memo
    form_class = MemoForm
    template_name = 'memo/edit_memo.html'
    context_object_name = 'memo'
    
    def get_success_url(self):
        return reverse_lazy('memo_detail', kwargs={'pk': self.object.pk})

class TagListView(ListView):
    model = Tag
    template_name = 'memo/tag_list.html'
    context_object_name = 'tags'

class TagCreateView(CreateView):
    model = Tag
    form_class = TagForm
    template_name = 'memo/tag_create.html'
    success_url = reverse_lazy('tag_list')

class MemoDetailView(DetailView):
    model = Memo
    template_name = 'memo/memo_detail.html'
    context_object_name = 'memo'

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['memo'].text = markdown.markdown(context['memo'].text, extensions=['nl2br'])
        return context

class TagView(ListView):
    model = Memo
    template_name = 'memo/tag_detail.html'
    context_object_name = 'memos'

    def get_queryset(self):
        self.tag = get_object_or_404(Tag, name=self.kwargs['tag_name'])
        return self.tag.memos.all()

    def get_context_data(self, **kwargs):
        context = super().get_context_data(**kwargs)
        context['tag'] = self.tag
        return context