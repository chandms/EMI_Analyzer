import { ComponentFixture, TestBed } from '@angular/core/testing';

import { SweepComponent } from './sweep.component';

describe('SweepComponent', () => {
  let component: SweepComponent;
  let fixture: ComponentFixture<SweepComponent>;

  beforeEach(async () => {
    await TestBed.configureTestingModule({
      declarations: [ SweepComponent ]
    })
    .compileComponents();
  });

  beforeEach(() => {
    fixture = TestBed.createComponent(SweepComponent);
    component = fixture.componentInstance;
    fixture.detectChanges();
  });

  it('should create', () => {
    expect(component).toBeTruthy();
  });
});
