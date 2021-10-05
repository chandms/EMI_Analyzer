import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProjectDescModalComponent } from './project-desc-modal.component';

describe('ProjectDescModalComponent', () => {
  let component: ProjectDescModalComponent;
  let fixture: ComponentFixture<ProjectDescModalComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ProjectDescModalComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ProjectDescModalComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
