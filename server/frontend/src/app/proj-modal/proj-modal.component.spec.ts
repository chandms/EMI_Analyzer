import { ComponentFixture, TestBed } from '@angular/core/testing';

import { ProjModalComponent } from './proj-modal.component';

describe('ProjModalComponent', () => {
  let component: ProjModalComponent;
  let fixture: ComponentFixture<ProjModalComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ ProjModalComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(ProjModalComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
